/*****************************************************************************
 * playlist/content.h
 *****************************************************************************
 * Copyright (C) 2018 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_PLAYLIST_CONTENT_H
#define VLC_PLAYLIST_CONTENT_H

typedef struct vlc_playlist vlc_playlist_t;
typedef struct input_item_t input_item_t;

/* called by vlc_playlist_Delete() in playlist.c */
void
vlc_playlist_ClearItems(vlc_playlist_t *playlist);

/* expand an item (replace it by the given media array) */
int
vlc_playlist_Expand(vlc_playlist_t *playlist, size_t index,
                    input_item_t *const media[], size_t count);

#endif
