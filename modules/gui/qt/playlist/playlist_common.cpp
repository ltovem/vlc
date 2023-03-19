// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/

#include "playlist_common.hpp"


PlaylistPtr::PlaylistPtr()
    : m_playlist(nullptr)
{}

PlaylistPtr::PlaylistPtr(vlc_playlist_t* pl)
    : m_playlist(pl)
{}

PlaylistPtr::PlaylistPtr(const PlaylistPtr& ptr)
    : m_playlist(ptr.m_playlist)
{
}

PlaylistPtr&PlaylistPtr::operator=(const PlaylistPtr& ptr)
{
    this->m_playlist = ptr.m_playlist;
    return *this;
}

