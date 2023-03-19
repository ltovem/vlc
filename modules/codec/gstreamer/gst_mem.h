// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * gst_mem.h: GStreamer Memory picture context for VLC
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Author: Yann Lochet <yann@l0chet.fr>
 *****************************************************************************/

#ifndef VLC_GST_MEM_H
#define VLC_GST_MEM_H

#include <vlc_picture.h>

#include <gst/gst.h>
#include <gst/video/video.h>

struct gst_mem_pic_context
{
    picture_context_t s;
    GstBuffer *p_buf;
    GstVideoInfo *p_vinfo;
};

#endif
