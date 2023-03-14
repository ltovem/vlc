/*****************************************************************************
 * gstcopypicture.h: copy GStreamer frames into pictures
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Author: Yann Lochet <yann@l0chet.fr>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_GSTCOPYPICTURE_H
#define VLC_GSTCOPYPICTURE_H

#include <gst/gst.h>
#include <gst/video/video.h>

void gst_CopyPicture( picture_t *p_pic, GstVideoFrame *p_frame );

#endif
