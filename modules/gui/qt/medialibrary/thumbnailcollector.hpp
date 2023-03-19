// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef THUMBNAILCOLLECTOR_HPP
#define THUMBNAILCOLLECTOR_HPP


#include <QObject>
#include <QHash>
#include <QSet>
#include <QMutex>
#include <memory>

#include <functional>

class MediaLib;
struct vlc_medialibrary_t;
struct vlc_ml_event_callback_t;
struct vlc_ml_event_t;


class ThumbnailCollector : public QObject
{
    Q_OBJECT

public:
    ThumbnailCollector(QObject *parent = nullptr);

    void start(MediaLib *ml, const QSet<int64_t> &mlIds);

    QHash<int64_t, QString> allGenerated() { return m_thumbnails; }

signals:
    void finished();

private:
    static void onVlcMLEvent(void *data, const vlc_ml_event_t *event);

    QMutex m_mut;
    MediaLib *m_ml {};
    std::unique_ptr<vlc_ml_event_callback_t,
                    std::function<void(vlc_ml_event_callback_t*)>> m_ml_event_handle {};

    QSet<int64_t> m_pending;
    QHash<int64_t, QString> m_thumbnails;
};


#endif // THUMBNAILCOLLECTOR_HPP
