/*****************************************************************************
 * playlist/player.h
 *****************************************************************************
 * Copyright (C) 2018 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_PLAYLIST_PLAYER_H
#define VLC_PLAYLIST_PLAYER_H

#include <vlc_common.h>

typedef struct vlc_playlist vlc_playlist_t;

bool
vlc_playlist_PlayerInit(vlc_playlist_t *playlist, vlc_object_t *parent);

void
vlc_playlist_PlayerDestroy(vlc_playlist_t *playlist);

#endif
