/*****************************************************************************
 * gstvlcpictureplaneallocator.h: VLC pictures wrapped by GstAllocator
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
#ifndef VLC_GST_PICTURE_PLANE_ALLOCATOR_H_
#define VLC_GST_PICTURE_PLANE_ALLOCATOR_H_

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideometa.h>

#include <vlc_common.h>
#include <vlc_codec.h>

typedef struct _GstVlcPicturePlane GstVlcPicturePlane;
typedef struct _GstVlcPicturePlaneAllocator GstVlcPicturePlaneAllocator;
typedef struct _GstVlcPicturePlaneAllocatorClass \
                                    GstVlcPicturePlaneAllocatorClass;

/* allocator functions */
#define GST_TYPE_VLC_PICTURE_PLANE_ALLOCATOR      \
    (gst_vlc_picture_plane_allocator_get_type())
#define GST_IS_VLC_PICTURE_PLANE_ALLOCATOR(obj)   \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_VLC_PICTURE_PLANE_ALLOCATOR))
#define GST_VLC_PICTURE_PLANE_ALLOCATOR(obj)      \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_VLC_PICTURE_PLANE_ALLOCATOR, \
    GstVlcPicturePlaneAllocator))
#define GST_VLC_PICTURE_PLANE_ALLOCATOR_CAST(obj) \
    ((GstVlcPicturePlaneAllocator*)(obj))

struct _GstVlcPicturePlane
{
    GstMemory parent;

    decoder_t *p_dec;
    picture_t *p_pic;
    plane_t *p_plane;
};

struct _GstVlcPicturePlaneAllocator
{
    GstAllocator parent;
    picture_t pic_info;;

    decoder_t *p_dec;
};

struct _GstVlcPicturePlaneAllocatorClass
{
    GstAllocatorClass parent_class;
};

GType gst_vlc_picture_plane_allocator_get_type( void );
GstVlcPicturePlaneAllocator* gst_vlc_picture_plane_allocator_new(
        decoder_t *p_dec );
bool gst_vlc_picture_plane_allocator_query_format(
        GstVlcPicturePlaneAllocator *p_gallocator, GstVideoInfo *p_info,
        GstVideoAlignment *p_align, GstCaps *p_caps );
bool gst_vlc_set_vout_fmt( GstVideoInfo *p_info, GstVideoAlignment *p_align,
        GstCaps *p_caps, decoder_t *p_dec );
vlc_fourcc_t gst_vlc_to_map_format( const char* psz_fourcc );
void gst_vlc_dec_ensure_empty_queue( decoder_t* p_dec );
bool gst_vlc_picture_plane_allocator_hold( GstVlcPicturePlaneAllocator
        *p_allocator, GstBuffer *p_buffer );
void gst_vlc_picture_plane_allocator_release(
        GstVlcPicturePlaneAllocator *p_allocator, GstBuffer *p_buffer );
bool gst_vlc_picture_plane_allocator_alloc(
        GstVlcPicturePlaneAllocator *p_allocator,
        GstBuffer *p_buffer );
#endif
