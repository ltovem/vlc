/*****************************************************************************
 * vlc_mime.h: Mime type recognition
 *****************************************************************************
 * Copyright (C) 2012 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_MIME_H
#define VLC_MIME_H 1

/**
 * \file
 * Mime type recognition helpers.
 */

VLC_API const char * vlc_mime_Ext2Mime( const char *psz_url );

#endif /* _VLC_MIME_H */
