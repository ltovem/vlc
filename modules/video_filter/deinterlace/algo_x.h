/*****************************************************************************
 * algo_x.h : "X" algorithm for vlc deinterlacer
 *****************************************************************************
 * Copyright (C) 2000-2011 VLC authors and VideoLAN
 *
 * Author: Laurent Aimar <fenrir@videolan.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_DEINTERLACE_ALGO_X_H
#define VLC_DEINTERLACE_ALGO_X_H 1

/* Forward declarations */
struct picture_t;

/*****************************************************************************
 * Functions
 *****************************************************************************/

/**
 * Interpolating deinterlace filter "X".
 *
 * The algorithm works on a 8x8 block basis; It copies the top field
 * and applies a process to recreate the bottom field.
 *
 * If a 8x8 block is classified as :
 *   - progressive: it applies a small blend (1,6,1)
 *   - interlaced:
 *    * in the SIMD version: we do a ME between the 2 fields, if there is a
 *      good match we use MC to recreate the bottom field (with a small
 *      blend (1,6,1) )
 *    * otherwise: it recreates the bottom field by an edge oriented
 *      interpolation.
 *
 * @param[in] p_pic Input frame.
 * @param[out] p_outpic Output frame. Must be allocated by caller.
 * @see Deinterlace()
 */
int RenderX( filter_t *, picture_t *p_outpic, picture_t *p_pic );

#endif
