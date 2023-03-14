/**
 * @file platform_android.c
 * @brief Vulkan platform-specific code for Android
 */
/*****************************************************************************
 * Copyright © 2018 Niklas Haas, Thomas Guillem
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>

#include "platform.h"
#include "../android/utils.h"

static void ClosePlatform(vlc_vk_platform_t *vk);
static int CreateSurface(vlc_vk_platform_t *vk, const vlc_vk_instance_t *, VkSurfaceKHR *);
static const struct vlc_vk_platform_operations platform_ops =
{
    .close = ClosePlatform,
    .create_surface = CreateSurface,
};

static int InitPlatform(vlc_vk_platform_t *vk)
{
    if (vk->window->type != VLC_WINDOW_TYPE_ANDROID_NATIVE)
        return VLC_EGENERIC;

    vk->platform_ext = VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
    vk->platform_ops = &platform_ops;
    return VLC_SUCCESS;
}

static void ClosePlatform(vlc_vk_platform_t *vk)
{
    AWindowHandler_releaseANativeWindow(vk->window->handle.anativewindow,
                                        AWindow_Video);
}

static int CreateSurface(vlc_vk_platform_t *vk, const vlc_vk_instance_t *inst,
                         VkSurfaceKHR *surface_out)
{
    ANativeWindow *anw =
        AWindowHandler_getANativeWindow(vk->window->handle.anativewindow,
                                        AWindow_Video);
    PFN_vkCreateAndroidSurfaceKHR CreateAndroidSurfaceKHR = (PFN_vkCreateAndroidSurfaceKHR)
        inst->get_proc_address(inst->instance, "vkCreateAndroidSurfaceKHR");

    VkAndroidSurfaceCreateInfoKHR ainfo = {
         .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
         .pNext = NULL,
         .flags = 0,
         .window = anw,
    };

    VkResult res = CreateAndroidSurfaceKHR(inst->instance, &ainfo, NULL, surface_out);
    if (res != VK_SUCCESS) {
        msg_Err(vk, "Failed creating Android surface");
        return VLC_EGENERIC;
    }

    return VLC_SUCCESS;
}

vlc_module_begin()
    set_shortname("Vulkan Android")
    set_description(N_("Android platform support for Vulkan"))
    set_subcategory(SUBCAT_VIDEO_VOUT)
    set_capability("vulkan platform", 50)
    set_callback(InitPlatform)
    add_shortcut("vk_android")
vlc_module_end()
