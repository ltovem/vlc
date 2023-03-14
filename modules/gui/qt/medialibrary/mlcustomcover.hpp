/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Prince Gupta <guptaprince8832@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef MLCUSTOMCOVER_HPP
#define MLCUSTOMCOVER_HPP

#include <QQuickAsyncImageProvider>

#include <memory>

class MLItemId;
class MediaLib;

class MLCustomCover : public QQuickAsyncImageProvider
{
public:
    MLCustomCover(const QString &providerId, MediaLib *ml);

    QString get(const MLItemId &parentId
                , const QSize &size
                , const QString &defaultCover
                , const int countX = 2
                , const int countY = 2
                , const int blur = 0
                , const bool split_duplicate = false);

    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize);

private:
    const QString m_providerId;
    MediaLib *m_ml = nullptr;
};

#endif // MLCUSTOMCOVER_HPP
