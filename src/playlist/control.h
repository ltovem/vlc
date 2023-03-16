// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * playlist/control.h
 *****************************************************************************
 * Copyright (C) 2018 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef VLC_PLAYLIST_CONTROL_H
#define VLC_PLAYLIST_CONTROL_H

#include <vlc_common.h>

typedef struct vlc_playlist vlc_playlist_t;

bool
vlc_playlist_ComputeHasPrev(vlc_playlist_t *playlist);

bool
vlc_playlist_ComputeHasNext(vlc_playlist_t *playlist);

int
vlc_playlist_SetCurrentMedia(vlc_playlist_t *playlist, ssize_t index);

input_item_t *
vlc_playlist_GetNextMedia(vlc_playlist_t *playlist);

#endif
