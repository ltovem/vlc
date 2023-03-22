/*****************************************************************************
 * libvlc_thumbnailer.h:  libvlc external API
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
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

#ifndef VLC_LIBVLC_THUMBNAILER_H
#define VLC_LIBVLC_THUMBNAILER_H 1

# ifdef __cplusplus
extern "C" {
# else
#  include <stdbool.h>
# endif

/** \defgroup libvlc_thumbnailer LibVLC thumbnailer
 * \ingroup libvlc
 * @{
 * \file
 * LibVLC thumbnailer
 */

/**
 * \brief libvlc_media_thumbnail_request_t An opaque thumbnail request object
 */
typedef struct libvlc_media_thumbnail_request_t libvlc_media_thumbnail_request_t;

typedef enum libvlc_thumbnailer_seek_speed_t
{
    libvlc_media_thumbnail_seek_precise,
    libvlc_media_thumbnail_seek_fast,
} libvlc_thumbnailer_seek_speed_t;

/**
 * \brief libvlc_media_request_thumbnail_by_time Start an asynchronous thumbnail generation
 *
 * If the request is successfully queued, the libvlc_MediaThumbnailGenerated is
 * guaranteed to be emitted (except if the request is destroyed early by the
 * user).
 * The resulting thumbnail size can either be:
 * - Hardcoded by providing both width & height. In which case, the image will
 *   be stretched to match the provided aspect ratio, or cropped if crop is true.
 * - Derived from the media aspect ratio if only width or height is provided and
 *   the other one is set to 0.
 *
 * \param inst LibVLC instance to generate the thumbnail with
 * \param md media descriptor object
 * \param time The time at which the thumbnail should be generated
 * \param speed The seeking speed \sa{libvlc_thumbnailer_seek_speed_t}
 * \param width The thumbnail width
 * \param height the thumbnail height
 * \param crop Should the picture be cropped to preserve source aspect ratio
 * \param picture_type The thumbnail picture type \sa{libvlc_picture_type_t}
 * \param timeout A timeout value in ms, or 0 to disable timeout
 *
 * \return A valid opaque request object, or NULL in case of failure.
 * It must be released by libvlc_media_thumbnail_request_destroy() and
 * can be cancelled by calling it early.
 *
 * \version libvlc 4.0.0 or later
 *
 * \see libvlc_picture_t
 * \see libvlc_picture_type_t
 */
LIBVLC_API libvlc_media_thumbnail_request_t*
libvlc_media_thumbnail_request_by_time( libvlc_instance_t *inst,
                                        libvlc_media_t *md, libvlc_time_t time,
                                        libvlc_thumbnailer_seek_speed_t speed,
                                        unsigned int width, unsigned int height,
                                        bool crop, libvlc_picture_type_t picture_type,
                                        libvlc_time_t timeout );

/**
 * \brief libvlc_media_request_thumbnail_by_pos Start an asynchronous thumbnail generation
 *
 * If the request is successfully queued, the libvlc_MediaThumbnailGenerated is
 * guaranteed to be emitted (except if the request is destroyed early by the
 * user).
 * The resulting thumbnail size can either be:
 * - Hardcoded by providing both width & height. In which case, the image will
 *   be stretched to match the provided aspect ratio, or cropped if crop is true.
 * - Derived from the media aspect ratio if only width or height is provided and
 *   the other one is set to 0.
 *
 * \param inst LibVLC instance to generate the thumbnail with
 * \param md media descriptor object
 * \param pos The position at which the thumbnail should be generated
 * \param speed The seeking speed \sa{libvlc_thumbnailer_seek_speed_t}
 * \param width The thumbnail width
 * \param height the thumbnail height
 * \param crop Should the picture be cropped to preserve source aspect ratio
 * \param picture_type The thumbnail picture type \sa{libvlc_picture_type_t}
 * \param timeout A timeout value in ms, or 0 to disable timeout
 *
 * \return A valid opaque request object, or NULL in case of failure.
 * It must be released by libvlc_media_thumbnail_request_destroy().
 *
 * \version libvlc 4.0.0 or later
 *
 * \see libvlc_picture_t
 * \see libvlc_picture_type_t
 */
LIBVLC_API libvlc_media_thumbnail_request_t*
libvlc_media_thumbnail_request_by_pos( libvlc_instance_t *inst,
                                       libvlc_media_t *md, double pos,
                                       libvlc_thumbnailer_seek_speed_t speed,
                                       unsigned int width, unsigned int height,
                                       bool crop, libvlc_picture_type_t picture_type,
                                       libvlc_time_t timeout );

/**
 * @brief libvlc_media_thumbnail_destroy destroys a thumbnail request
 * @param p_req An opaque thumbnail request object.
 *
 * This will also cancel the thumbnail request, no events will be emitted after
 * this call.
 */
LIBVLC_API void
libvlc_media_thumbnail_request_destroy( libvlc_media_thumbnail_request_t *p_req );

/** @}*/

# ifdef __cplusplus
}
# endif

#endif /* VLC_LIBVLC_THUMBNAILER_H */
