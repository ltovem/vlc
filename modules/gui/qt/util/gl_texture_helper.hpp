/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
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

#ifndef GLTEXTUREHELPER_HPP
#define GLTEXTUREHELPER_HPP

#include <QImage>
#include <QImageReader>
#include <QOpenGLTexture>
#include <QOpenGLContext>
#include <QDir>

#include <memory>

namespace GLTextureHelper
{
template<class GLFuncs>
std::unique_ptr<QOpenGLTexture> imageToCompressedTexture(const QImage& image, GLFuncs* funcs);

template<class GLFuncs>
bool saveCompressedTexture(QOpenGLTexture* texture, const QDir& directory, const QString& fileName, GLFuncs* funcs);

// Load image, convert to compressed texture
// and save the compressed texture
void generateAndSaveTexture(const QString& imagePath);
void generateAndSaveTextures(const QVector<QString>& imagePaths);
}

#endif // GLTEXTUREHELPER_HPP
