// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * attachments.h : MP4 attachments handling
 *****************************************************************************
 * Copyright (C) 2001-2015 VLC authors and VideoLAN
 *               2019 VideoLabs
 *****************************************************************************/
#ifndef VLC_MP4_ATTACHMENTS_H_
#define VLC_MP4_ATTACHMENTS_H_

size_t MP4_GetAttachments( const MP4_Box_t *, input_attachment_t *** );
const MP4_Box_t *MP4_GetMetaRoot( const MP4_Box_t *, const char ** );
int MP4_GetCoverMetaURI( const MP4_Box_t *,  const MP4_Box_t *,
                         const char *, vlc_meta_t * );

#endif
