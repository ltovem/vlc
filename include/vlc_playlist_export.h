/*****************************************************************************
 * vlc_playlist_export.h
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_PLAYLIST_EXPORT_H
#define VLC_PLAYLIST_EXPORT_H

#include <vlc_playlist.h>

/** API for playlist export modules */

/**
 * Opaque structure giving a read-only view of a playlist.
 *
 * The view is only valid until the playlist lock is released.
 */
struct vlc_playlist_view;

/**
 * Return the number of items in the view.
 *
 * The underlying playlist must be locked.
 *
 * \param view the playlist view
 */
VLC_API size_t
vlc_playlist_view_Count(struct vlc_playlist_view *view);

/**
 * Return the item at a given index.
 *
 * The index must be in range (less than vlc_playlist_view_Count()).
 *
 * The underlying playlist must be locked.
 *
 * \param view  the playlist view
 * \param index the index
 * \return the playlist item
 */
VLC_API vlc_playlist_item_t *
vlc_playlist_view_Get(struct vlc_playlist_view *view, size_t index);

/**
 * Structure received by playlist export module.
 */
struct vlc_playlist_export
{
    struct vlc_object_t obj;
    char *base_url;
    FILE *file;
    struct vlc_playlist_view *playlist_view;
};

#endif
