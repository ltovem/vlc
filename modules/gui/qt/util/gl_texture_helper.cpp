/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 * Copyright (C) 2017 The Qt Company Ltd
 * Copyright (C) 2013 Klaralvdalens Datakonsult AB (KDAB)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "gl_texture_helper.hpp"

#include <QOffscreenSurface>
#include <QSurfaceFormat>
#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLPixelTransferOptions>
#include <QByteArray>
#include <QThread>

#include <cstring>

namespace {

class GLTexture : public QOpenGLTexture
{
public:
    explicit GLTexture(Target target)
        : QOpenGLTexture(target) { };

    // The only difference with setData() is that
    // the internal format can be different than
    // what is read from the image.
    void setDataWithFormat(const QImage& image, MipMapGeneration genMipMaps, TextureFormat format)
    {
        QOpenGLContext *context = QOpenGLContext::currentContext();
        if (!context) {
            qWarning("GLTexture::setDataWithFormat() requires a valid current context");
            return;
        }
        if (image.isNull()) {
            qWarning("GLTexture::setDataWithFormat() tried to set a null image");
            return;
        }

        setFormat(format);

        setSize(image.width(), image.height());
        setMipLevels(genMipMaps == GenerateMipMaps ? maximumMipLevels() : 1);
        allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);
        // Upload pixel data and generate mipmaps
        QImage glImage = image.convertToFormat(QImage::Format_RGBA8888);
        QOpenGLPixelTransferOptions uploadOptions;
        uploadOptions.setAlignment(1);

        // Upload uncompressed image data, and request the OpenGL
        // driver to convert to the specified format internally
        setData(0, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, glImage.constBits(), &uploadOptions);
    }
};

}

void GLTextureHelper::generateAndSaveTexture(const QString& imagePath)
{
    return generateAndSaveTextures({imagePath});
}

void GLTextureHelper::generateAndSaveTextures(const QVector<QString>& imagePaths)
{
    // Do it in separate thread, so that the rendering in
    // Qt Scene Graph is not affected
    const auto thread = QThread::create([imagePaths]() {
        // Create an offscreen surface:
        QSurfaceFormat surfaceFormat;

        if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
        {
            // OpenGL proper
            // Use BPTC compression with OpenGL 4.2 (no extension required)
            // S3TC compression requires extension.
            // ASTC compression requires extension.
            // ETC/ETC2 compression does not require extension, however
            // desktop GPUs seem to be emulating it, except for Intel GPUs.
            surfaceFormat.setVersion(4, 2);
            surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
            surfaceFormat.setRenderableType(QSurfaceFormat::OpenGL);
            // Setting version to 4.5, and using OpenGL DSA functions might be a good idea...
        }
        else
        {
            // OpenGL ES and others are not supported
            return;
        }

        QOffscreenSurface surface;
        surface.setFormat(surfaceFormat);

        // As per documentation, some platforms require calling create()
        // and destroy() in GUI thread:
        QMetaObject::invokeMethod(qApp, [&surface]() {
                surface.create();
            }, Qt::BlockingQueuedConnection);

        if (!surface.isValid())
            return;

        QOpenGLContext ctx;
        ctx.setFormat(surfaceFormat);

        if (ctx.create())
        {
            // If context version is lower than requested,
            // we can not continue
            if (ctx.format().version() < surfaceFormat.version())
                return;
        }
        else
            return;

        if (!ctx.makeCurrent(&surface))
            return;

        const auto funcs = ctx.versionFunctions<QOpenGLFunctions_4_2_Core>();

        funcs->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (const auto& i : imagePaths)
        {
            QImageReader reader(i);

            if (!reader.canRead())
                continue;

            const auto texture = imageToCompressedTexture(reader.read(), funcs);

            if (!texture)
                continue;

            QFileInfo info(reader.fileName());
            saveCompressedTexture(texture.get(), info.dir(), info.baseName(), funcs);
        }

        ctx.doneCurrent();

        QMetaObject::invokeMethod(qApp, [&surface]() {
                surface.destroy();
            }, Qt::BlockingQueuedConnection);
    });

    thread->start(QThread::LowPriority);
}

template<class GLFuncs>
bool GLTextureHelper::saveCompressedTexture(QOpenGLTexture *texture, const QDir& directory, const QString& fileName, GLFuncs *funcs)
{
    assert(funcs);
    assert(texture);

    // Only KTX1 texture container format is supported
    QFile file(directory.filePath(fileName + QLatin1String(".ktx")));

    if (!file.open(QFile::WriteOnly))
        return false;

    texture->bind();

    const int mipLevel = texture->mipLevels();

    struct KTXHeader {
        quint8 identifier[12] { (quint8)'\xAB', 'K', 'T', 'X', ' ', '1', '1', (quint8)'\xBB', '\r', '\n', '\x1A', '\n' }; // KTX 1.1
        quint32 endianness = 0x04030201; // platform endian identifier
        quint32 glType = 0; // must be 0 for compressed textures
        quint32 glTypeSize = 1; // must be 1 for compressed textures
        quint32 glFormat = 0; // must be 0 for compressed textures
        quint32 glInternalFormat;
        quint32 glBaseInternalFormat;
        quint32 pixelWidth;
        quint32 pixelHeight;
        quint32 pixelDepth = 0; // must be 0 for 2D textures
        quint32 numberOfArrayElements = 0; // 0 for non array textures
        quint32 numberOfFaces = 1; // 1 for non cubemap faces
        quint32 numberOfMipmapLevels;
        quint32 bytesOfKeyValueData = 0; // Take offset into account if this is changed
    };

    struct KTXMipmapLevel {
        quint32 imageSize;
    };

    QVector<KTXMipmapLevel> levels(mipLevel);

    int fileSize = sizeof(KTXHeader);

    for (int i = 0; i < mipLevel; ++i)
    {
        int size;
        funcs->glGetTexLevelParameteriv(GL_TEXTURE_2D, i, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &size);
        fileSize += sizeof(KTXMipmapLevel) + size;
        levels[i].imageSize = size;
    }

    QByteArray data(fileSize, 0);

    char * pData = data.data();

    {
        KTXHeader hdr;
        hdr.glInternalFormat = texture->format();
        hdr.glBaseInternalFormat = GL_RGBA;
        hdr.pixelWidth = texture->width();
        hdr.pixelHeight = texture->height();
        hdr.numberOfMipmapLevels = mipLevel;
        std::memcpy(pData, &hdr, sizeof(KTXHeader));
        pData += sizeof(KTXHeader);
    }

    for (int i = 0; i < levels.size(); ++i)
    {
        std::memcpy(pData, &levels[i], sizeof(KTXMipmapLevel));
        pData += sizeof(KTXMipmapLevel);
        funcs->glGetCompressedTexImage(GL_TEXTURE_2D, i, pData);
        pData += levels[i].imageSize;
    }

    assert((pData - data.data()) == fileSize);

    return (file.write(data) == fileSize);
}

template<class GLFuncs>
std::unique_ptr<QOpenGLTexture> GLTextureHelper::imageToCompressedTexture(const QImage &image, GLFuncs* funcs)
{
    assert(funcs);
    std::unique_ptr<QOpenGLTexture> texture = std::make_unique<GLTexture>(GLTexture::Target2D);

    funcs->glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
    static_cast<GLTexture*>(texture.get())->setDataWithFormat(image,
                                                              /* QSG does not seem to fully support compressed mipmapped textures */
                                                              GLTexture::DontGenerateMipMaps,
                                                              GLTexture::TextureFormat::RGB_BP_UNorm);

    texture->bind();

    int r;
    funcs->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &r);

    if (r == GL_TRUE)
        return texture;
    else
    {
        qDebug("Failed to compress texture.");
        return nullptr;
    }
}
