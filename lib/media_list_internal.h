/*****************************************************************************
 * media_list_internal.h : Definition of opaque structures for libvlc exported API
 * Also contains some internal utility functions
 *****************************************************************************
 * Copyright (C) 2005-2009 VLC authors and VideoLAN
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef _LIBVLC_MEDIA_LIST_INTERNAL_H
#define _LIBVLC_MEDIA_LIST_INTERNAL_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc/vlc.h>
#include <vlc/libvlc_media.h>

#include <vlc_common.h>

struct libvlc_media_list_t
{
    libvlc_event_manager_t      event_manager;
    vlc_mutex_t                 object_lock;
    vlc_atomic_rc_t             rc;
    libvlc_media_t * p_md; /* The media from which the
                                       * mlist comes, if any. */
    libvlc_media_t * p_internal_md; /* media set from media.c */
    vlc_array_t                items;

    /* This indicates if this media list is read-only
     * from a user point of view */
    bool                  b_read_only;
};

/* Media List */
void libvlc_media_list_internal_add_media(
        libvlc_media_list_t * p_mlist,
        libvlc_media_t * p_md );

void libvlc_media_list_internal_insert_media(
        libvlc_media_list_t * p_mlist,
        libvlc_media_t * p_md, int index );

int libvlc_media_list_internal_remove_index(
        libvlc_media_list_t * p_mlist, int index );

void libvlc_media_list_internal_end_reached(
        libvlc_media_list_t * p_mlist );

#endif
