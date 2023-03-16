// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * window.h : Header for FFT window routines
 *****************************************************************************
 * Copyright (C) 2014 Ronald Wright
 *
 * Author: Ronald Wright <logiconcepts819@gmail.com>
 *****************************************************************************/

#ifndef VLC_VISUAL_WINDOW_H_
#define VLC_VISUAL_WINDOW_H_

#include <vlc_common.h>

/* Window type enum */
enum _enum_window_type { NONE, HANN, FLATTOP, BLACKMANHARRIS, KAISER };

/* Window context structure */
struct _struct_window_context {

    /* Storage for window function values */
    float * pf_window_table;

    /* Buffer size for the window */
    int i_buffer_size;
};

typedef enum _enum_window_type window_type;

/* Window parameter structure */
struct _struct_window_param {

    /* Window type */
    window_type wind_type;

    /* Kaiser window parameter */
    float f_kaiser_alpha;
};

/* Prototypes for the window function */
typedef struct _struct_window_context window_context;
typedef struct _struct_window_param window_param;
void window_get_param( vlc_object_t * p_aout, window_param * p_param );
bool window_init( int i_buffer_size, window_param * p_param,
                  window_context * p_ctx );
void window_scale_in_place( int16_t * p_buffer, window_context * p_ctx );
void window_close( window_context * p_ctx );

/* Macro for defining a new window context */
#define DEFINE_WIND_CONTEXT(name) window_context name = {NULL, 0}

#endif /* include-guard */
