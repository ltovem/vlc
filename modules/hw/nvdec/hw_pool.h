// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * hw_pool.h: hw based picture pool
 *****************************************************************************
 * Copyright (C) 2019-2020 VLC authors and VideoLAN
 *
 * Authors: Jai Luthra <me@jailuthra.in>
 *          Quentin Chateau <quentin.chateau@deepskycorp.com>
 *          Steve Lhomme <robux4@videolabs.io>
 *****************************************************************************/
#ifndef HW_POOL_H
#define HW_POOL_H

#include <vlc_picture.h>
#include <vlc_codec.h>

typedef struct hw_pool_t  hw_pool_t;
typedef struct hw_pool_owner  hw_pool_owner_t;

struct hw_pool_owner
{
    void *sys;
    void (*release_resources)(hw_pool_owner_t *, void *buffers[], size_t pics_count);
    picture_context_t * (*attach_picture)(hw_pool_owner_t *, hw_pool_t *, void *surface);
};

hw_pool_t* hw_pool_Create(hw_pool_owner_t *,
                          const video_format_t *, vlc_video_context *,
                          void *buffers[], size_t pics_count);
void hw_pool_AddRef(hw_pool_t *);
void hw_pool_Release(hw_pool_t *);

/**
 * Wait for a new picture to be available from the pool.
 *
 * The picture.p_sys is always NULL.
 */
picture_t* hw_pool_Wait(hw_pool_t *);

#endif // HW_POOL_H
