/*****************************************************************************
 * art.h
 *****************************************************************************
 * Copyright (C) 1999-2008 VLC authors and VideoLAN
 *
 * Authors: Samuel Hocevar <sam@zoy.org>
 *          Clément Stenac <zorglub@videolan.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef _INPUT_ART_H
#define _INPUT_ART_H 1

int input_FindArtInCache( input_item_t * );
int input_FindArtInCacheUsingItemUID( input_item_t * );

int input_SaveArt( vlc_object_t *, input_item_t *,
                   const void *, size_t, const char *psz_type );

#endif

