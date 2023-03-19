// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * chroma.h: decoder and encoder using libavcodec
 *****************************************************************************
 * Copyright (C) 2001-2008 VLC authors and VideoLAN
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *****************************************************************************/

/* VLC <-> avutil tables */

#ifndef VLC_AVUTIL_CHROMA_H_
#define VLC_AVUTIL_CHROMA_H_

#include <libavutil/pixfmt.h>

enum AVPixelFormat FindFfmpegChroma( vlc_fourcc_t );
int GetFfmpegChroma( enum AVPixelFormat *i_ffmpeg_chroma, const video_format_t *fmt );

vlc_fourcc_t FindVlcChroma( enum AVPixelFormat );
int GetVlcChroma( video_format_t *fmt, enum AVPixelFormat i_ffmpeg_chroma );

#endif
