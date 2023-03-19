// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * adjust_sat_hue.h : Hue/Saturation executive part of adjust plugin for vlc
 *****************************************************************************
 * Copyright (C) 2011 VideoLAN
 *
 * Authors: Simon Latapie <garf@via.ecp.fr>
 *          Antoine Cellerier <dionoea -at- videolan d0t org>
 *          Martin Briza <gamajun@seznam.cz> (SSE)
 *****************************************************************************/

#include <vlc_common.h>
#include <vlc_cpu.h>

/**
 * Functions processing saturation and hue of adjust filter.
 * Prototype and parameters stay the same across different variations.
 *
 * @param p_pic Source picture
 * @param p_outpic Destination picture
 * @param i_sin Sinus value of hue
 * @param i_cos Cosinus value of hue
 * @param i_sat Saturation
 * @param i_x Additional value of saturation
 * @param i_y Additional value of saturation
 */

/**
 * Basic C compiler generated function for planar format, i_sat > 256
 */
int planar_sat_hue_clip_C( picture_t * p_pic, picture_t * p_outpic,
                           int i_sin, int i_cos, int i_sat, int i_x, int i_y );

/**
 * Basic C compiler generated function for planar format, i_sat <= 256
 */
int planar_sat_hue_C( picture_t * p_pic, picture_t * p_outpic,
                      int i_sin, int i_cos, int i_sat, int i_x, int i_y );
/**
 * Basic C compiler generated function for {9,10}-bit planar format, i_sat > {512,1024}
 */
int planar_sat_hue_clip_C_16( picture_t * p_pic, picture_t * p_outpic,
        int i_sin, int i_cos, int i_sat, int i_x, int i_y );

/**
 * Basic C compiler generated function for {9,10}-bit planar format, i_sat <= {512,1024}
 */
int planar_sat_hue_C_16( picture_t * p_pic, picture_t * p_outpic,
        int i_sin, int i_cos, int i_sat, int i_x, int i_y );


/**
 * Basic C compiler generated function for packed format, i_sat > 256
 */
int packed_sat_hue_clip_C( picture_t * p_pic, picture_t * p_outpic,
                           int i_sin, int i_cos, int i_sat, int i_x, int i_y );

/**
 * Basic C compiler generated function for packed format, i_sat <= 256
 */
int packed_sat_hue_C( picture_t * p_pic, picture_t * p_outpic,
                      int i_sin, int i_cos, int i_sat, int i_x, int i_y );
