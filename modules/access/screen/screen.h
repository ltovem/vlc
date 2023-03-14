/*****************************************************************************
 * screen.h: Screen capture module.
 *****************************************************************************
 * Copyright (C) 2004-2008 VLC authors and VideoLAN
 *
 * Authors: Gildas Bazin <gbazin@videolan.org>
 *          Antoine Cellerier <dionoea at videolan dot org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#include <vlc_input.h>
#include <vlc_access.h>
#include <vlc_demux.h>

#define SCREEN_SUBSCREEN
#ifdef _WIN32
#define SCREEN_MOUSE
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct screen_data_t screen_data_t;

struct screen_capture_operations
{
    block_t* (*capture)( demux_t * );
    void (*close)( screen_data_t * );
};

typedef struct
{
    es_format_t fmt;
    es_out_id_t *es;

    float f_fps;
    vlc_tick_t i_next_date;
    vlc_tick_t i_incr;

    vlc_tick_t i_start;

#ifdef SCREEN_SUBSCREEN
    bool b_follow_mouse;
    unsigned int i_screen_height;
    unsigned int i_screen_width;

    unsigned int i_top;
    unsigned int i_left;
    unsigned int i_height;
    unsigned int i_width;
#endif

#ifdef SCREEN_MOUSE
    picture_t *p_mouse;
    picture_t dst;
#endif

    screen_data_t *p_data;
    const struct screen_capture_operations *ops;
} demux_sys_t;

int      screen_InitCapture ( demux_t * );
#if defined(_WIN32) && !defined(VLC_WINSTORE_APP)
int      screen_InitCaptureGDI ( demux_t * );
#endif

#ifdef SCREEN_SUBSCREEN
void FollowMouse( demux_sys_t *, int, int );
#endif

#ifdef __cplusplus
}
#endif
