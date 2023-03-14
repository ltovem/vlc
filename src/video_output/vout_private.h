/*****************************************************************************
 * vout_private.h : Internal vout definitions
 *****************************************************************************
 * Copyright (C) 2008-2018 VLC authors and VideoLAN
 * Copyright (C) 2008 Laurent Aimar
 *
 * Authors: Laurent Aimar < fenrir _AT_ videolan _DOT_ org >
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef LIBVLC_VOUT_PRIVATE_H
#define LIBVLC_VOUT_PRIVATE_H 1

#include <vlc_picture_fifo.h>
#include <vlc_picture_pool.h>
#include <vlc_vout_display.h>

typedef struct vout_thread_private_t vout_thread_private_t;

/* */
struct vout_thread_private_t
{
    struct {
        bool        is_interlaced;
        bool        has_deint;
        vlc_tick_t  date;
    } interlacing;

    picture_pool_t  *private_pool;
    picture_pool_t  *display_pool;
};

/* */
vout_display_t *vout_OpenWrapper(vout_thread_t *, vout_thread_private_t *, const char *,
                     const vout_display_cfg_t *, const video_format_t *, vlc_video_context *);
void vout_CloseWrapper(vout_thread_t *, vout_thread_private_t *, vout_display_t *vd);

void vout_InitInterlacingSupport(vout_thread_t *, vout_thread_private_t *);
void vout_ReinitInterlacingSupport(vout_thread_t *, vout_thread_private_t *);
void vout_SetInterlacingState(vout_thread_t *, vout_thread_private_t *, bool is_interlaced);

#endif // LIBVLC_VOUT_PRIVATE_H
