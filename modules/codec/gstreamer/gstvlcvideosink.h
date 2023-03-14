/*****************************************************************************
 * gstvlcvideosink.h: VLC gstreamer video sink
 *****************************************************************************
 * Copyright (C) 2016 VLC authors and VideoLAN
 *
 * Author: Vikram Fugro <vikram.fugro@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#ifndef VLC_GST_VIDEO_SINK_H
#define VLC_GST_VIDEO_SINK_H

#include <gst/gst.h>
#include <gst/gstallocator.h>

#include <gst/video/video.h>
#include <gst/video/gstvideometa.h>
#include <gst/base/gstbasesink.h>

#include <vlc_codec.h>

typedef struct _GstVlcVideoSink GstVlcVideoSink;
typedef struct _GstVlcVideoSinkClass GstVlcVideoSinkClass;

#define GST_TYPE_VLC_VIDEO_SINK \
    (gst_vlc_video_sink_get_type())
#define GST_VLC_VIDEO_SINK(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_VLC_VIDEO_SINK, \
        GstVlcVideoSink))
#define GST_VLC_VIDEO_SINK_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_VLC_VIDEO_SINK, \
        GstVlcVideoSinkClass))
#define GST_IS_VLC_VIDEO_SINK(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_VLC_VIDEO_SINK))
#define GST_IS_VLC_VIDEO_SINK_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_VLC_VIDEO_SINK))

struct _GstVlcVideoSink
{
    GstBaseSink parent;

    GstAllocator *p_allocator;
    GstVideoInfo vinfo;
    gboolean b_use_pool;

    decoder_t *p_dec;

    //FIXME: caps_signal
    gboolean (*new_caps) ( GstElement *p_ele, GstCaps *p_caps,
            gpointer p_data );
};

struct _GstVlcVideoSinkClass
{
    GstBaseSinkClass parent_class;

    //FIXME: caps_signal
#if 0
    gboolean (*new_caps) ( GstElement *p_ele, GstCaps *p_caps,
            gpointer p_data );
#endif
    void (*new_buffer) ( GstElement *p_ele, GstBuffer *p_buffer,
            gpointer p_data );
};

GType gst_vlc_video_sink_get_type (void);

gboolean gst_vlc_video_sink_query_caps( GstQuery *p_query );

#endif /* __GST_VLC_VIDEO_SINK_H__ */
