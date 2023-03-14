/*****************************************************************************
 * vlc_viewpoint.h: viewpoint struct and helpers
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_VIEWPOINT_H_
#define VLC_VIEWPOINT_H_ 1

#include <vlc_common.h>

#include <math.h>

/**
 * \file
 * Video and audio viewpoint struct and helpers
 */

#define FIELD_OF_VIEW_DEGREES_DEFAULT  80.f
#define FIELD_OF_VIEW_DEGREES_MAX 150.f
#define FIELD_OF_VIEW_DEGREES_MIN 20.f

/**
 * Viewpoints
 */
struct vlc_viewpoint_t {
    float yaw;   /* yaw in degrees */
    float pitch; /* pitch in degrees */
    float roll;  /* roll in degrees */
    float fov;   /* field of view in degrees */
};

static inline void vlc_viewpoint_init( vlc_viewpoint_t *p_vp )
{
    p_vp->yaw = p_vp->pitch = p_vp->roll = 0.0f;
    p_vp->fov = FIELD_OF_VIEW_DEGREES_DEFAULT;
}

static inline void vlc_viewpoint_clip( vlc_viewpoint_t *p_vp )
{
    p_vp->yaw = fmodf( p_vp->yaw, 360.f );
    p_vp->pitch = fmodf( p_vp->pitch, 360.f );
    p_vp->roll = fmodf( p_vp->roll, 360.f );
    p_vp->fov = VLC_CLIP( p_vp->fov, FIELD_OF_VIEW_DEGREES_MIN,
                          FIELD_OF_VIEW_DEGREES_MAX );
}

/**
 * Generate the 4x4 transform matrix corresponding to a viewpoint
 *
 * Convert a vlc_viewpoint_t into a 4x4 transform matrix with a column-major
 * layout.
 *
 * \param vp a valid viewpoint object
 * \param matrix a 4x4-sized array which will contain the matrix data
 */
VLC_API
void vlc_viewpoint_to_4x4( const vlc_viewpoint_t *vp, float *matrix );

#endif /* VLC_VIEWPOINT_H_ */
