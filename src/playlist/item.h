// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * playlist/item.h
 *****************************************************************************
 * Copyright (C) 2018 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef VLC_PLAYLIST_ITEM_H
#define VLC_PLAYLIST_ITEM_H

#include <vlc_atomic.h>

typedef struct vlc_playlist_item vlc_playlist_item_t;
typedef struct input_item_t input_item_t;

struct vlc_playlist_item
{
    input_item_t *media;
    uint64_t id;
    vlc_atomic_rc_t rc;
};

/* _New() is private, it is called when inserting new media in the playlist */
vlc_playlist_item_t *
vlc_playlist_item_New(input_item_t *media, uint64_t id);

#endif
