// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * events.h: Windows video output header file
 *****************************************************************************
 * Copyright (C) 2001-2009 VLC authors and VideoLAN
 *
 * Authors: Gildas Bazin <gbazin@videolan.org>
 *          Damien Fouilleul <damienf@videolan.org>
 *****************************************************************************/

#include <vlc_window.h>
#include <vlc_vout_display.h>

/**
 * HWNDs manager.
 */
typedef struct event_thread_t event_thread_t;

typedef struct {
    bool is_projected;
    unsigned width;
    unsigned height;
} event_cfg_t;

typedef struct {
    vlc_window_t *parent_window;
    HWND hparent;
    HWND hvideownd;
} event_hwnd_t;

event_thread_t *EventThreadCreate( vlc_object_t *, vlc_window_t *);
void            EventThreadDestroy( event_thread_t * );
int             EventThreadStart( event_thread_t *, event_hwnd_t *, const event_cfg_t * );
void            EventThreadStop( event_thread_t * );
