// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * instance.h: libplacebo instance abstraction
 *****************************************************************************
 * Copyright (C) 2021 Niklas Haas
 *****************************************************************************/

#ifndef VLC_LIBPLACEBO_INSTANCE_H
#define VLC_LIBPLACEBO_INSTANCE_H

#include <vlc_common.h>
#include <vlc_vout_display.h>

#include <libplacebo/swapchain.h>
#include <libplacebo/log.h>
#include <libplacebo/gpu.h>

struct vlc_placebo_t;
struct vlc_placebo_operations
{
    void (*close)(struct vlc_placebo_t *);

    // For acquiring/releasing the context on the current thread. (Optional)
    int (*make_current)(struct vlc_placebo_t *);
    void (*release_current)(struct vlc_placebo_t *);
};

typedef struct vlc_placebo_system_t vlc_placebo_system_t;

// Shared struct for libplacebo context / gpu / swapchain
typedef struct vlc_placebo_t
{
    // fields internal to instance.c, should not be touched
    struct vlc_object_t obj;
    vlc_placebo_system_t *sys;

    pl_log log;
    pl_gpu gpu;
    pl_swapchain swapchain;

    const struct vlc_placebo_operations *ops;
} vlc_placebo_t;

vlc_placebo_t *vlc_placebo_Create(const vout_display_cfg_t *, const char*) VLC_USED;
void vlc_placebo_Release(vlc_placebo_t *);

// Needed around every `pl_gpu` / `pl_swapchain` operation
int vlc_placebo_MakeCurrent(vlc_placebo_t *);
void vlc_placebo_ReleaseCurrent(vlc_placebo_t *);

#endif // VLC_LIBPLACEBO_INSTANCE_H
