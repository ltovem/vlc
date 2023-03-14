/*****************************************************************************
 * control.h : vout internal control
 *****************************************************************************
 * Copyright (C) 2009-2010 Laurent Aimar
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef LIBVLC_VOUT_INTERNAL_CONTROL_H
#define LIBVLC_VOUT_INTERNAL_CONTROL_H

#include <vlc_viewpoint.h>

/* */
typedef struct {
    vlc_mutex_t lock;
    vlc_cond_t wait_request;
    vlc_cond_t wait_available; /* available: yielding && !is_held */

    /* */
    bool forced_awake;
    bool yielding;
    bool is_held;
    unsigned pending_count;
} vout_control_t;

/* */
void vout_control_Init(vout_control_t *);

/* controls outside of the vout thread */
void vout_control_Wake(vout_control_t *);
void vout_control_Hold(vout_control_t *);
void vout_control_Release(vout_control_t *);
void vout_control_ReleaseAndWake(vout_control_t *);

/* control inside of the vout thread */
void vout_control_Wait(vout_control_t *, vlc_tick_t deadline);

#endif
