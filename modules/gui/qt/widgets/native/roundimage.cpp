/*****************************************************************************
 * roundimage.cpp: Custom widgets
 ****************************************************************************
 * Copyright (C) 2021 the VideoLAN team
 *
 * Authors: Prince Gupta <guptaprince8832@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt.hpp"

#include "roundimage.hpp"
#include "roundimage_p.hpp"
#include "util/asynctask.hpp"
#include "util/qsgroundedrectangularimagenode.hpp"

#include <qhashfunctions.h>

#include <QBuffer>
#include <QFile>
#include <QImage>
#include <QImageReader>
#include <QPainter>
#include <QPainterPath>
#include <QQuickWindow>
#include <QGuiApplication>
#include <QSGImageNode>
#include <QtQml>
#include <QQmlFile>

#ifdef QT_NETWORK_LIB
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#endif


bool operator ==(const ImageCacheKey &lhs, const ImageCacheKey &rhs)
{
    return lhs.radius == rhs.radius && lhs.size == rhs.size && lhs.url == rhs.url;
}

uint qHash(const ImageCacheKey &key, uint seed)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, key.url);
    seed = hash(seed, key.size.width());
    seed = hash(seed, key.size.height());
    seed = hash(seed, key.radius);
    return seed;
}

namespace
{
    QImage prepareImage(const QSize &targetSize, const qreal radius, const QImage sourceImage)
    {
        if (qFuzzyIsNull(radius))
        {
            return sourceImage.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }

        QImage target(targetSize, QImage::Format_ARGB32_Premultiplied);
        if (target.isNull())
            return target;

        target.fill(Qt::transparent);

        {
            QPainter painter(&target);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

            QPainterPath path;
            path.addRoundedRect(0, 0, targetSize.width(), targetSize.height(), radius, radius);
            painter.setClipPath(path);

            // do PreserveAspectCrop
            const auto imageSize = sourceImage.size();
            const QPointF alignedCenteredTopLeft {(targetSize.width() - imageSize.width()) / 2.
                                                 , (targetSize.height() - imageSize.height()) / 2.};
            painter.drawImage(QRectF {alignedCenteredTopLeft, imageSize}, sourceImage);
        }

        return target;
    }

    class ImageReader : public AsyncTask<QImage>
    {
    public:
        // requestedSize is only taken as hint, the Image is resized with PreserveAspectCrop
        ImageReader(QIODevice *device, QSize requestedSize, const qreal radius)
            : device {device}
            , requestedSize {requestedSize}
            , radius {radius}
        {
        }

        QString errorString() const { return errorStr; }

        QImage execute() override
        {
            QImageReader reader;
            reader.setDevice(device);
            const QSize sourceSize = reader.size();

            if (requestedSize.isValid())
                reader.setScaledSize(sourceSize.scaled(requestedSize, Qt::KeepAspectRatioByExpanding));

            auto img = reader.read();
            errorStr = reader.errorString();

            if (!errorStr.isEmpty())
                img = prepareImage(requestedSize.isValid() ? requestedSize : img.size(), radius, img);

            return img;
        }

    private:
        QIODevice *device;
        QSize requestedSize;
        qreal radius;
        QString errorStr;
    };

    class LocalImageResponse : public QQuickImageResponse
    {
    public:
        LocalImageResponse(const QString &fileName, const QSize &requestedSize, const qreal radius)
        {
            auto file = new QFile(fileName);
            reader.reset(new ImageReader(file, requestedSize, radius));
            file->setParent(reader.get());

            connect(reader.get(), &ImageReader::result, this, &LocalImageResponse::handleImageRead);

            reader->start(*QThreadPool::globalInstance());
        }

        QQuickTextureFactory *textureFactory() const override
        {
            return result.isNull() ? nullptr : QQuickTextureFactory::textureFactoryForImage(result);
        }

        QString errorString() const override
        {
            return errorStr;
        }

    private:
        void handleImageRead()
        {
            result = reader->takeResult();
            errorStr = reader->errorString();
            reader.reset();

            emit finished();
        }

        QImage result;
        TaskHandle<ImageReader> reader;
        QString errorStr;
    };

#ifdef QT_NETWORK_LIB
    class NetworkImageResponse : public QQuickImageResponse
    {
    public:
        NetworkImageResponse(QNetworkReply *reply, QSize requestedSize, const qreal radius)
            : reply {reply}
            , requestedSize {requestedSize}
            , radius {radius}
        {
            QObject::connect(reply, &QNetworkReply::finished
                             , this, &NetworkImageResponse::handleNetworkReplyFinished);
        }

        QQuickTextureFactory *textureFactory() const override
        {
            return result.isNull() ? nullptr : QQuickTextureFactory::textureFactoryForImage(result);
        }

        QString errorString() const override
        {
            return error;
        }

        void cancel() override
        {
            if (reply->isRunning())
                reply->abort();

            reader.reset();
        }

    private:
        void handleNetworkReplyFinished()
        {
            if (reply->error() != QNetworkReply::NoError)
            {
                error = reply->errorString();
                emit finished();
                return;
            }

            reader.reset(new ImageReader(reply, requestedSize, radius));
            QObject::connect(reader.get(), &ImageReader::result, this, [this]()
            {
                result = reader->takeResult();
                error = reader->errorString();
                reader.reset();

                emit finished();
            });

            reader->start(*QThreadPool::globalInstance());
        }

        QNetworkReply *reply;
        QSize requestedSize;
        qreal radius;
        TaskHandle<ImageReader> reader;
        QImage result;
        QString error;
    };
#endif

    // adapts a given QQuickImageResponse to produce a image with radius
    class ImageResponseRadiusAdaptor : public QQuickImageResponse
    {
    public:
        ImageResponseRadiusAdaptor(QQuickImageResponse *response, const qreal radius) : response {response}, radius {radius}
        {
            response->setParent(this);
            connect(response, &QQuickImageResponse::finished
                    , this, &ImageResponseRadiusAdaptor::handleResponseFinished);
        }

        QString errorString() const override { return errStr; }

        QQuickTextureFactory *textureFactory() const override
        {
            return !result.isNull() ? QQuickTextureFactory::textureFactoryForImage(result) : nullptr;
        }

    private:
        class RoundImageGenerator : public AsyncTask<QImage>
        {
        public:
            RoundImageGenerator(const QImage &img, const qreal radius) : sourceImg {img}, radius{radius}
            {
            }

            QImage execute() override
            {
                return prepareImage(sourceImg.size(), radius, sourceImg);
            }

        private:
            QImage sourceImg;
            qreal radius;
        };

        void handleResponseFinished()
        {
            errStr = response->errorString();
            auto textureFactory = std::unique_ptr<QQuickTextureFactory>(response->textureFactory());
            auto img = !textureFactory ? QImage {} : textureFactory->image();
            if (!textureFactory || img.isNull())
                return;

            response->disconnect(this);
            response->deleteLater();
            response = nullptr;

            generator.reset(new RoundImageGenerator(img, radius));
            connect(generator.get(), &RoundImageGenerator::result
                    , this, &ImageResponseRadiusAdaptor::handleGeneratorFinished);

            generator->start(*QThreadPool::globalInstance());
        }

        void handleGeneratorFinished()
        {
            result = generator->takeResult();

            emit finished();
        }

        QQuickImageResponse *response;
        QString errStr;
        qreal radius;
        QImage result;
        TaskHandle<RoundImageGenerator> generator;
    };

// global RoundImage cache
RoundImageCache g_imageCache = {};

void clearCache()
{
    g_imageCache.clear();
}

QQuickWindow* firstTopLevelQuickWindow()
{
    QQuickWindow* window = nullptr;
    const auto topLevelWindows = qGuiApp->topLevelWindows();
    for (const auto i : topLevelWindows)
    {
        if (qobject_cast<QQuickWindow*>(i))
        {
            window = static_cast<QQuickWindow*>(i);
            break;
        }
    }
    return window;
}

}

// RoundImageCache

RoundImageCache::RoundImageCache()
    : m_imageCache(32 * 1024 * 1024 / 4) // 32 MiB
{}

std::shared_ptr<RoundImageRequest> RoundImageCache::requestImage(const ImageCacheKey& key, qreal dpr, QQmlEngine *engine)
{
    //do we already have a pending request?
    auto it = m_pendingRequests.find(key);
    if (it != m_pendingRequests.end())
    {
        std::shared_ptr<RoundImageRequest> request = it->second.lock();
        if (request)
            return request;
    }
    auto request = std::make_shared<RoundImageRequest>(key, dpr, engine);
    if (request->getStatus() == RoundImage::Status::Error)
        return {};
    m_pendingRequests[key] = request;
    return request;
}

void RoundImageCache::removeRequest(const ImageCacheKey& key)
{
    m_pendingRequests.erase(key);
}

// RoundImageRequest

RoundImageRequest::RoundImageRequest(const ImageCacheKey& key, qreal dpr, QQmlEngine *engine)
    : m_key(key)
    , m_dpr(dpr)
{
    m_imageResponse = getAsyncImageResponse(key.url, key.size, key.radius, engine);
    if (m_imageResponse)
        connect(m_imageResponse, &QQuickImageResponse::finished, this, &RoundImageRequest::handleImageResponseFinished);
    else
        m_status = RoundImage::Error;
}

RoundImageRequest::~RoundImageRequest()
{
    if (m_imageResponse)
    {
        if (m_cancelOnDelete)
            m_imageResponse->cancel();

        m_imageResponse->deleteLater();
    }
    g_imageCache.removeRequest(m_key);
}

void RoundImageRequest::handleImageResponseFinished()
{
    m_cancelOnDelete = false;
    g_imageCache.removeRequest(m_key);

    const QString error = m_imageResponse->errorString();
    std::shared_ptr<QSGTexture> texture;

    if (auto textureFactory = m_imageResponse->textureFactory())
    {
        static QQuickWindow* const window = firstTopLevelQuickWindow();
        assert(window);

        connect(window, &QQuickWindow::sceneGraphInvalidated, window, clearCache, static_cast<Qt::ConnectionType>(Qt::DirectConnection | Qt::UniqueConnection));

        auto image = textureFactory->image();
        image.setDevicePixelRatio(m_dpr);

        QQuickWindow::CreateTextureOptions flags = QQuickWindow::TextureCanUseAtlas;
        if (!image.hasAlphaChannel())
            flags |= QQuickWindow::TextureIsOpaque;

        // We are safe, QQuickWindow::createTextureFromImage can be called from any thread.
        // Since textures do not have window affinity, we can use textures generated
        // by different windows within the items that belong to other windows.
        texture = std::shared_ptr<QSGTexture>(window->createTextureFromImage(textureFactory->image(), flags));
        if (Q_UNLIKELY(!texture))
            qmlWarning(window->contentItem()) << "Could not generate texture from " << image;

        delete textureFactory;
    }

    if (!texture || texture->textureSize().isEmpty())
    {
        qDebug() << "failed to get image, error" << error << m_key.url;
        m_status = RoundImage::Status::Error;
        emit requestCompleted(m_status, {});
        return;
    }

    g_imageCache.insert(m_key, texture, texture->textureSize().width() * texture->textureSize().height());
    emit requestCompleted(RoundImage::Status::Ready, texture);
}


QQuickImageResponse* RoundImageRequest::getAsyncImageResponse(const QUrl &url, const QSize &requestedSize, const qreal radius, QQmlEngine *engine)
{
    if (url.scheme() == QStringLiteral("image"))
    {
        auto provider = engine->imageProvider(url.host());
        if (!provider)
            return nullptr;

        assert(provider->imageType() == QQmlImageProviderBase::ImageResponse);

        const auto imageId = url.toString(QUrl::RemoveScheme | QUrl::RemoveAuthority).mid(1);

        if (provider->imageType() == QQmlImageProviderBase::ImageResponse)
        {
            auto rawImageResponse = static_cast<QQuickAsyncImageProvider *>(provider)->requestImageResponse(imageId, requestedSize);
            return new ImageResponseRadiusAdaptor(rawImageResponse, radius);
        }

        return nullptr;
    }
    else if (QQmlFile::isLocalFile(url))
    {
        return new LocalImageResponse(QQmlFile::urlToLocalFileOrQrc(url), requestedSize, radius);
    }
#ifdef QT_NETWORK_LIB
    else
    {
        QNetworkRequest request(url);
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        auto reply = engine->networkAccessManager()->get(request);
        return new NetworkImageResponse(reply, requestedSize, radius);
    }
#else
    return nullptr;
#endif
}

// RoundImage

RoundImage::RoundImage(QQuickItem *parent) : QQuickItem {parent}
{
    if (Q_LIKELY(qGuiApp))
        setDPR(qGuiApp->devicePixelRatio());

    connect(this, &QQuickItem::heightChanged, this, &RoundImage::regenerateRoundImage);
    connect(this, &QQuickItem::widthChanged, this, &RoundImage::regenerateRoundImage);

    connect(this, &QQuickItem::windowChanged, this, [this](const QQuickWindow* const window) {
        if (window)
            setDPR(window->devicePixelRatio());
        else if (Q_LIKELY(qGuiApp))
            setDPR(qGuiApp->devicePixelRatio());
    });

    connect(this, &QQuickItem::windowChanged, this, &RoundImage::adjustQSGCustomGeometry);
}

RoundImage::~RoundImage()
{
}

QSGNode *RoundImage::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    auto customImageNode = dynamic_cast<QSGRoundedRectangularImageNode*>(oldNode);
    auto imageNode = dynamic_cast<QSGImageNode*>(oldNode);

    if (Q_UNLIKELY(oldNode && ((m_QSGCustomGeometry && !customImageNode)
                               || (!m_QSGCustomGeometry && !imageNode))))
    {
        // This must be extremely unlikely.
        // Assigned to different window with different renderer?
        delete oldNode;
        oldNode = nullptr;
    }

    if (!m_texture || m_texture->textureSize().isEmpty())
    {
        delete oldNode;
        m_dirty = false;
        return nullptr;
    }

    if (!oldNode)
    {
        if (m_QSGCustomGeometry)
        {
            customImageNode = new QSGRoundedRectangularImageNode;
        }
        else
        {
            assert(window());
            imageNode = window()->createImageNode();
            assert(imageNode);
            imageNode->setOwnsTexture(false);
        }

        m_dirty = true;
    }

    if (m_dirty)
    {
        m_dirty = false;
        assert(m_texture);

        if (m_QSGCustomGeometry)
            customImageNode->setTexture(m_texture);
        else
            imageNode->setTexture(m_texture.get());
    }

    // Geometry:
    if (m_QSGCustomGeometry)
    {
        customImageNode->setShape({boundingRect(), radius()});
        customImageNode->setSmooth(smooth());
        assert(customImageNode->geometry() && customImageNode->material());
        return customImageNode;
    }
    else
    {
        imageNode->setRect(boundingRect());
        imageNode->setFiltering(smooth() ? QSGTexture::Linear : QSGTexture::Nearest);
        return imageNode;
    }
}

void RoundImage::componentComplete()
{
    QQuickItem::componentComplete();

    if (!m_source.isEmpty())
        regenerateRoundImage();
}

QUrl RoundImage::source() const
{
    return m_source;
}

qreal RoundImage::radius() const
{
    return m_radius;
}

RoundImage::Status RoundImage::status() const
{
    return m_status;
}

void RoundImage::setSource(const QUrl& source)
{
    if (m_source == source)
        return;

    m_source = source;
    emit sourceChanged(m_source);
    regenerateRoundImage();
}

void RoundImage::setRadius(qreal radius)
{
    if (m_radius == radius)
        return;

    m_radius = radius;
    emit radiusChanged(m_radius);
}

void RoundImage::itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &value)
{
    if (change == QQuickItem::ItemDevicePixelRatioHasChanged)
        setDPR(value.realValue);

    QQuickItem::itemChange(change, value);
}

void RoundImage::setDPR(const qreal value)
{
    if (m_dpr == value)
        return;

    m_dpr = value;
    regenerateRoundImage();
}

void RoundImage::load()
{
    m_enqueuedGeneration = false;

    auto engine = qmlEngine(this);
    if (!engine || m_source.isEmpty() || !size().isValid() || size().isEmpty())
        return;

    const qreal scaledWidth = this->width() * m_dpr;
    const qreal scaledHeight = this->height() * m_dpr;
    const qreal scaledRadius = m_QSGCustomGeometry ? 0.0 : (this->radius() * m_dpr);

    const ImageCacheKey key {source(), QSizeF {scaledWidth, scaledHeight}.toSize(), scaledRadius};

    if (const auto image = g_imageCache.object(key)) // should only by called in mainthread
    {
        onRequestCompleted(Status::Ready, image);
        return;
    }

    m_activeImageResponse = g_imageCache.requestImage(key, m_dpr, engine);
    if (!m_activeImageResponse)
    {
        onRequestCompleted(RoundImage::Error, {});
        return;
    }

    connect(m_activeImageResponse.get(), &RoundImageRequest::requestCompleted, this, &RoundImage::onRequestCompleted);
    //at this point m_activeImageResponse is either in Loading or Error status
    onRequestCompleted(RoundImage::Loading, {});
}

void RoundImage::onRequestCompleted(Status status, const std::shared_ptr<QSGTexture>& texture)
{
    setRoundImage(texture);
    switch (status)
    {
    case RoundImage::Error:
        setStatus(Status::Error);
        m_activeImageResponse.reset();
        break;
    case RoundImage::Ready:
    {
        if (!texture || texture->textureSize().isEmpty())
            setStatus(Status::Error);
        else
            setStatus(Status::Ready);
        m_activeImageResponse.reset();
        break;
    }
    case RoundImage::Loading:
        setStatus(Status::Loading);
        break;
    case RoundImage::Null:
        //requests should not be yield this state
        vlc_assert_unreachable();
    }
}

void RoundImage::setRoundImage(const std::shared_ptr<QSGTexture> &texture)
{
    m_dirty = true;
    m_texture = texture;

    static QQuickWindow* const topWindow = firstTopLevelQuickWindow();
    connect(topWindow, &QQuickWindow::sceneGraphInvalidated, this, &RoundImage::clear, static_cast<Qt::ConnectionType>(Qt::DirectConnection | Qt::UniqueConnection));

    // remove old contents, setting ItemHasContent to false will
    // inhibit updatePaintNode() call and old content will remain
    if (!texture || texture->textureSize().isEmpty())
        update();

    setFlag(ItemHasContents, texture && !texture->textureSize().isEmpty());
    update();
}

void RoundImage::setStatus(const RoundImage::Status status)
{
    if (status == m_status)
        return;

    m_status = status;
    emit statusChanged();
}

void RoundImage::regenerateRoundImage()
{
    if (!isComponentComplete() || m_enqueuedGeneration)
        return;

    setStatus(source().isEmpty() ? Status::Null : Status::Loading);

    // remove old contents
    setRoundImage({});

    m_activeImageResponse.reset();

    // use Qt::QueuedConnection to delay generation, so that dependent properties
    // subsequent updates can be merged, f.e when VLCStyle.scale changes
    m_enqueuedGeneration = true;

    QMetaObject::invokeMethod(this, &RoundImage::load, Qt::QueuedConnection);
}

void RoundImage::adjustQSGCustomGeometry(const QQuickWindow* const window)
{
    if (!window) return;

    // No need to check if the scene graph is initialized according to docs.

    const auto enableCustomGeometry = [this, window]() {
        if (m_QSGCustomGeometry)
            return;

        // Favor custom geometry instead of clipping the image.
        // This allows making the texture opaque, as long as
        // source image is also opaque, for optimization
        // purposes.
        if (window->format().samples() != -1)
            m_QSGCustomGeometry = true;
        // No need to regenerate as transparent part will not
        // matter. However, in order for the material to not
        // require blending, a regeneration is necessary.
        // We could force the material to not require blending
        // for the outer transparent part which is not within the
        // geometry, but then inherently transparent images would
        // not be rendered correctly due to the alpha channel.

        QMetaObject::invokeMethod(this,
                                  &RoundImage::regenerateRoundImage,
                                  Qt::QueuedConnection);

        // It might be tempting to not regenerate the image on size
        // change. However;
        // If upscaled, we don't know if the image can be provided
        // in a higher resolution.
        // If downscaled, we would like to free some used memory.
        // On the other hand, there is no need to regenerate the
        // image when the radius changes. This behavior does not
        // mean that the radius can be animated. Although possible,
        // the custom geometry node is not designed to handle animations.
        disconnect(this, &RoundImage::radiusChanged, this, &RoundImage::regenerateRoundImage);
        connect(this, &RoundImage::radiusChanged, this, &QQuickItem::update);
    };

    const auto disableCustomGeometry = [this]() {
        if (!m_QSGCustomGeometry)
            return;

        m_QSGCustomGeometry = false;

        QMetaObject::invokeMethod(this,
                                  &RoundImage::regenerateRoundImage,
                                  Qt::QueuedConnection);

        connect(this, &RoundImage::radiusChanged, this, &RoundImage::regenerateRoundImage);
        disconnect(this, &RoundImage::radiusChanged, this, &QQuickItem::update);
    };


    if (window->rendererInterface()->graphicsApi() == QSGRendererInterface::GraphicsApi::OpenGL)
    {
        // Direct OpenGL, enable custom geometry:
        enableCustomGeometry();
    }
    else
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        // QSG(Opaque)TextureMaterial supports Qt RHI,
        // so there is no obstacle using custom geometry
        // if Qt RHI is in use.
        if (QSGRendererInterface::isApiRhiBased(window->rendererInterface()->graphicsApi()))
            enableCustomGeometry();
        else
            disableCustomGeometry();
#else
        // Qt RHI is introduced in Qt 5.14.
        // QSG(Opaque)TextureMaterial does not support any graphics API other than OpenGL
        // without the Qt RHI abstraction layer.
        disableCustomGeometry();
#endif
    }
}
