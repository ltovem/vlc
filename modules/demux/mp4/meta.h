/*****************************************************************************
 * meta.h: mp4 meta handling
 *****************************************************************************
 * Copyright (C) 2001-2004, 2010, 2014 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef VLC_MP4_META_H_
#define VLC_MP4_META_H_

void SetupMeta( vlc_meta_t *p_meta, const MP4_Box_t *p_udta );

#endif
