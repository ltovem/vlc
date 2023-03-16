// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * vlc_avcodec.h: VLC thread support for libavcodec
 *****************************************************************************
 * Copyright (C) 2009-2010 Rémi Denis-Courmont
 *****************************************************************************/

#ifndef VLC_AVCODEC_H
# define VLC_AVCODEC_H 1

static inline void vlc_avcodec_lock (void)
{
    vlc_global_lock (VLC_AVCODEC_MUTEX);
}

static inline void vlc_avcodec_unlock (void)
{
    vlc_global_unlock (VLC_AVCODEC_MUTEX);
}

#endif
