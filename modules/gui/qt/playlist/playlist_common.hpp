/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef PLAYLIST_COMMON_HPP
#define PLAYLIST_COMMON_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <QObject>
#include <vlc_playlist.h>

// QObject wrapper to carry playlist ptr through QML
class PlaylistPtr
{
    Q_GADGET
public:
    PlaylistPtr();
    PlaylistPtr(vlc_playlist_t* pl);
    PlaylistPtr(const PlaylistPtr& ptr);
    PlaylistPtr& operator=(const PlaylistPtr& ptr);

    vlc_playlist_t* m_playlist = nullptr;
};

class PlaylistLocker
{
public:
    inline PlaylistLocker(vlc_playlist_t* playlist)
        : m_playlist(playlist)
    {
        vlc_playlist_Lock(m_playlist);
    }

    inline  ~PlaylistLocker()
    {
        vlc_playlist_Unlock(m_playlist);
    }

    vlc_playlist_t* m_playlist;
};

#endif // PLAYLIST_COMMON_HPP
