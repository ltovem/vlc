// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * control.c : vout internal control
 *****************************************************************************
 * Copyright (C) 2009 Laurent Aimar
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_vout.h>
#include "vout_internal.h"
#include "control.h"

/* */
void vout_control_Init(vout_control_t *ctrl)
{
    vlc_mutex_init(&ctrl->lock);
    vlc_cond_init(&ctrl->wait_request);
    vlc_cond_init(&ctrl->wait_available);

    ctrl->is_held = false;
    ctrl->yielding = false;
    ctrl->forced_awake = false;
    ctrl->pending_count = 0;
}

void vout_control_Wake(vout_control_t *ctrl)
{
    vlc_mutex_lock(&ctrl->lock);
    ctrl->forced_awake = true;
    vlc_cond_signal(&ctrl->wait_request);
    vlc_mutex_unlock(&ctrl->lock);
}

void vout_control_Hold(vout_control_t *ctrl)
{
    vlc_mutex_lock(&ctrl->lock);
    ++ctrl->pending_count;
    vlc_cond_signal(&ctrl->wait_request);
    while (ctrl->is_held || !ctrl->yielding)
        vlc_cond_wait(&ctrl->wait_available, &ctrl->lock);
    ctrl->is_held = true;
    --ctrl->pending_count;
    if (ctrl->pending_count == 0)
        vlc_cond_signal(&ctrl->wait_request);
    vlc_mutex_unlock(&ctrl->lock);
}

static void vout_control_ReleaseUnlocked(vout_control_t *ctrl)
{
    assert(ctrl->is_held);
    ctrl->is_held = false;
    vlc_cond_signal(&ctrl->wait_available);
}

void vout_control_Release(vout_control_t *ctrl)
{
    vlc_mutex_lock(&ctrl->lock);
    vout_control_ReleaseUnlocked(ctrl);
    vlc_mutex_unlock(&ctrl->lock);
}

void vout_control_ReleaseAndWake(vout_control_t *ctrl)
{
    vlc_mutex_lock(&ctrl->lock);
    vout_control_ReleaseUnlocked(ctrl);
    ctrl->forced_awake = true;
    vlc_cond_signal(&ctrl->wait_request);
    vlc_mutex_unlock(&ctrl->lock);
}

void vout_control_Wait(vout_control_t *ctrl, vlc_tick_t deadline)
{
    vlc_mutex_lock(&ctrl->lock);

    ctrl->yielding = true;

    while (ctrl->pending_count != 0)
    {
        /* Let vout_control_Hold() callers pass */
        vlc_cond_signal(&ctrl->wait_available);
        vlc_cond_wait(&ctrl->wait_request, &ctrl->lock);
    }

    if (deadline != VLC_TICK_INVALID)
    {
        do
        {
            if (ctrl->forced_awake)
                break;

            vlc_cond_signal(&ctrl->wait_available);
        }
        while (vlc_cond_timedwait(&ctrl->wait_request, &ctrl->lock,
                                  deadline) == 0);
    }

    ctrl->yielding = false;

    while (ctrl->is_held)
        vlc_cond_wait(&ctrl->wait_available, &ctrl->lock);

    ctrl->forced_awake = false;
    vlc_mutex_unlock(&ctrl->lock);
}

