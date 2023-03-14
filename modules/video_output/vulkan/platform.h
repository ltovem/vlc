/*****************************************************************************
 * platform.h: Vulkan platform abstraction
 *****************************************************************************
 * Copyright (C) 2018 Niklas Haas
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_VULKAN_PLATFORM_H
#define VLC_VULKAN_PLATFORM_H

#include <vlc_common.h>
#include <vlc_window.h>

#include "instance.h"

struct vlc_vk_platform_t;
struct vlc_vk_platform_operations
{
    void (*close)(struct vlc_vk_platform_t *);
    int (*create_surface)(struct vlc_vk_platform_t *, const vlc_vk_instance_t *, VkSurfaceKHR *);
};


// Struct for platform-specific Vulkan state
typedef struct vlc_vk_platform_t
{
    // set by platform.c
    struct vlc_object_t obj;
    struct vlc_window *window;
    module_t *module;

    // set by the platform
    void *platform_sys;
    const char *platform_ext;
    const struct vlc_vk_platform_operations *ops;
} vlc_vk_platform_t;

vlc_vk_platform_t *vlc_vk_platform_Create(struct vlc_window *, const char *) VLC_USED;
void vlc_vk_platform_Release(vlc_vk_platform_t *);

// Create a vulkan surface and store it to `surface_out`
static inline int vlc_vk_CreateSurface(vlc_vk_platform_t * vk,
                                       const vlc_vk_instance_t *instance,
                                       VkSurfaceKHR *surface_out)
{
    return vk->ops->create_surface(vk, instance, surface_out);
}

static inline void vlc_vk_DestroySurface(const vlc_vk_instance_t *inst,
                                         VkSurfaceKHR surface)
{
    PFN_vkDestroySurfaceKHR DestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)
        inst->get_proc_address(inst->instance, "vkDestroySurfaceKHR");

    DestroySurfaceKHR(inst->instance, surface, NULL);
}

#endif // VLC_VULKAN_PLATFORM_H
