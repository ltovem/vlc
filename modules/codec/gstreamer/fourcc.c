// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * fourcc.c: convert between gst <->  vlc formats
 *****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * Author: Vikram Fugro <vikram.fugro@gmail.com>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_codec.h>

vlc_fourcc_t GetGstVLCFourcc( const char* gst );

typedef struct
{
    char gst [10];
    vlc_fourcc_t i_fourcc;
} gst_vlc_rawvideo_fourcc;

/*
 * Raw Video Formats
 */
static const gst_vlc_rawvideo_fourcc raw_video_fmts[] =
{
    // NOTE: These should be sorted entries, keyed by `gst` field
    // cat entries | tr -dc "[:alnum:][:space:]_" | sort -n -k1 | xargs printf "{ \"%s\", %s },\n"
    { "I420_10BE", VLC_CODEC_I420_10B },
    { "I420_10LE", VLC_CODEC_I420_10L },
    { "I420_12BE", VLC_CODEC_I420_12B },
    { "I420_12LE", VLC_CODEC_I420_12L },
    { "I420_16BE", VLC_CODEC_I420_16B },
    { "I420_16LE", VLC_CODEC_I420_16L },
    { "I420_9BE", VLC_CODEC_I420_9B },
    { "I420_9LE", VLC_CODEC_I420_9L },
    { "I422_10BE", VLC_CODEC_I422_10B },
    { "I422_10LE", VLC_CODEC_I422_10L },
    { "I422_12BE", VLC_CODEC_I422_12B },
    { "I422_12LE", VLC_CODEC_I422_12L },
    { "I422_16BE", VLC_CODEC_I422_16B },
    { "I422_16LE", VLC_CODEC_I422_16L },
    { "I422_9BE", VLC_CODEC_I422_9B },
    { "I422_9LE", VLC_CODEC_I422_9L },
    { "I444_10BE", VLC_CODEC_I444_10B },
    { "I444_10LE", VLC_CODEC_I444_10L },
    { "I444_12BE", VLC_CODEC_I444_12B },
    { "I444_12LE", VLC_CODEC_I444_12L },
    { "I444_16BE", VLC_CODEC_I444_16B },
    { "I444_16LE", VLC_CODEC_I444_16L },
    { "I444_9BE", VLC_CODEC_I444_9B },
    { "I444_9LE", VLC_CODEC_I444_9L },
};

static int compare_func( const void* key, const void* ent )
{
    return strcmp( (char*)key, ((gst_vlc_rawvideo_fourcc*)ent)->gst );
}

vlc_fourcc_t GetGstVLCFourcc( const char* gst )
{
    gst_vlc_rawvideo_fourcc* found = NULL;

    if( !gst )
    {
        return VLC_CODEC_UNKNOWN;
    }

    found = bsearch( gst, raw_video_fmts,
                ARRAY_SIZE(raw_video_fmts), sizeof(gst_vlc_rawvideo_fourcc),
                compare_func );

    if( !found )
        return VLC_CODEC_UNKNOWN;
    else
        return found->i_fourcc;
}
