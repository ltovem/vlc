/*****************************************************************************
 * thumbnailer.c: Libvlc thumbnailer API
 *****************************************************************************
 * Copyright (C) 2024 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>
#include <errno.h>
#include <limits.h>

#include <vlc/libvlc.h>
#include <vlc/libvlc_picture.h>
#include <vlc/libvlc_media.h>
#include <vlc/libvlc_events.h>

#include <vlc_common.h>
#include <vlc_thumbnailer.h>

#include "../src/libvlc.h"

#include "libvlc_internal.h"
#include "media_internal.h"
#include "picture_internal.h"

struct libvlc_media_thumbnail_request_t
{
    libvlc_instance_t *instance;
    libvlc_media_t *md;
    unsigned int width;
    unsigned int height;
    bool crop;
    libvlc_picture_type_t type;
    vlc_thumbnailer_request_t* req;
};

static void media_on_thumbnail_ready( void* data, picture_t* thumbnail )
{
    libvlc_media_thumbnail_request_t *req = data;
    libvlc_media_t *p_media = req->md;
    libvlc_event_t event;
    event.type = libvlc_MediaThumbnailGenerated;
    libvlc_picture_t* pic = NULL;
    if ( thumbnail != NULL )
        pic = libvlc_picture_new( VLC_OBJECT(req->instance->p_libvlc_int),
                                    thumbnail, req->type, req->width, req->height,
                                    req->crop );
    event.u.media_thumbnail_generated.p_thumbnail = pic;
    libvlc_event_send( &p_media->event_manager, &event );
    if ( pic != NULL )
        libvlc_picture_release( pic );
}

// Start an asynchronous thumbnail generation
libvlc_media_thumbnail_request_t*
libvlc_media_thumbnail_request_by_time( libvlc_instance_t *inst,
                                        libvlc_media_t *md, libvlc_time_t time,
                                        libvlc_thumbnailer_seek_speed_t speed,
                                        unsigned int width, unsigned int height,
                                        bool crop, libvlc_picture_type_t picture_type,
                                        libvlc_time_t timeout )
{
    assert( md );

    libvlc_priv_t *p_priv = libvlc_priv(inst->p_libvlc_int);
    if( unlikely( p_priv->p_thumbnailer == NULL ) )
        return NULL;

    libvlc_media_thumbnail_request_t *req = malloc( sizeof( *req ) );
    if ( unlikely( req == NULL ) )
        return NULL;

    req->instance = inst;
    req->md = md;
    req->width = width;
    req->height = height;
    req->type = picture_type;
    req->crop = crop;
    libvlc_media_retain( md );
    req->req = vlc_thumbnailer_RequestByTime( p_priv->p_thumbnailer,
        vlc_tick_from_libvlc_time( time ),
        speed == libvlc_media_thumbnail_seek_fast ?
            VLC_THUMBNAILER_SEEK_FAST : VLC_THUMBNAILER_SEEK_PRECISE,
        md->p_input_item,
        timeout > 0 ? vlc_tick_from_libvlc_time( timeout ) : VLC_TICK_INVALID,
        media_on_thumbnail_ready, req );
    if ( req->req == NULL )
    {
        free( req );
        libvlc_media_release( md );
        return NULL;
    }
    libvlc_retain(inst);
    return req;
}

// Start an asynchronous thumbnail generation
libvlc_media_thumbnail_request_t*
libvlc_media_thumbnail_request_by_pos( libvlc_instance_t *inst,
                                       libvlc_media_t *md, double pos,
                                       libvlc_thumbnailer_seek_speed_t speed,
                                       unsigned int width, unsigned int height,
                                       bool crop, libvlc_picture_type_t picture_type,
                                       libvlc_time_t timeout )
{
    assert( md );

    libvlc_priv_t *priv = libvlc_priv(inst->p_libvlc_int);
    if( unlikely( priv->p_thumbnailer == NULL ) )
        return NULL;

    libvlc_media_thumbnail_request_t *req = malloc( sizeof( *req ) );
    if ( unlikely( req == NULL ) )
        return NULL;

    req->instance = inst;
    req->md = md;
    req->width = width;
    req->height = height;
    req->crop = crop;
    req->type = picture_type;
    libvlc_media_retain( md );
    req->req = vlc_thumbnailer_RequestByPos( priv->p_thumbnailer, pos,
        speed == libvlc_media_thumbnail_seek_fast ?
            VLC_THUMBNAILER_SEEK_FAST : VLC_THUMBNAILER_SEEK_PRECISE,
        md->p_input_item,
        timeout > 0 ? vlc_tick_from_libvlc_time( timeout ) : VLC_TICK_INVALID,
        media_on_thumbnail_ready, req );
    if ( req->req == NULL )
    {
        free( req );
        libvlc_media_release( md );
        return NULL;
    }
    libvlc_retain(inst);
    return req;
}

// Destroy a thumbnail request
void libvlc_media_thumbnail_request_destroy( libvlc_media_thumbnail_request_t *req )
{
    libvlc_priv_t *p_priv = libvlc_priv(req->instance->p_libvlc_int);
    assert( p_priv->p_thumbnailer != NULL );
    vlc_thumbnailer_CancelRequest( p_priv->p_thumbnailer, req->req );
    vlc_thumbnailer_request_Destroy( req->req );
    libvlc_media_release( req->md );
    libvlc_release(req->instance);
    free( req );
}
