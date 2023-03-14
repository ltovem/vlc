/*****************************************************************************
 * playlist_model_p.hpp
 *****************************************************************************
 * Copyright (C) 2018 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef PLAYLIST_MODEL_P_HPP
#define PLAYLIST_MODEL_P_HPP

#include "playlist_model.hpp"

namespace vlc {
namespace playlist {

class PlaylistListModelPrivate
{
    Q_DISABLE_COPY(PlaylistListModelPrivate)
public:
    Q_DECLARE_PUBLIC(PlaylistListModel)
    PlaylistListModel* const q_ptr;

public:
    PlaylistListModelPrivate(PlaylistListModel* playlistList);
    ~PlaylistListModelPrivate();

    ///call function @a fun on object thread
    template <typename Fun>
    inline void callAsync(Fun&& fun)
    {
        Q_Q(PlaylistListModel);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        QMetaObject::invokeMethod(q, std::forward<Fun>(fun), Qt::QueuedConnection, nullptr);
#else
        QObject src;
        QObject::connect(&src, &QObject::destroyed, q, std::forward<Fun>(fun), Qt::QueuedConnection);
#endif
    }

    void onItemsReset(const QVector<PlaylistItem>& items);
    void onItemsAdded(const QVector<PlaylistItem>& added, size_t index);
    void onItemsMoved(size_t index, size_t count, size_t target);
    void onItemsRemoved(size_t index, size_t count);

    void notifyItemsChanged(int index, int count,
                            const QVector<int> &roles = {});

    vlc_playlist_t* m_playlist = nullptr;
    vlc_playlist_listener_id *m_listener = nullptr;

    /* access only from the UI thread */
    QVector<PlaylistItem> m_items;
    ssize_t m_current = -1;

    vlc_tick_t m_duration = 0;
};

} //namespace playlist
} //namespace vlc


#endif // PLAYLIST_MODEL_P_HPP
