// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * instance.h: Vulkan instance abstraction
 *****************************************************************************
 * Copyright (C) 2018 Niklas Haas
 *****************************************************************************/

#ifndef VLC_VULKAN_INSTANCE_H
#define VLC_VULKAN_INSTANCE_H

#include <vlc_common.h>
#include <vulkan/vulkan.h>

typedef struct vlc_vk_instance_t
{
    VkInstance instance;
    PFN_vkGetInstanceProcAddr get_proc_address;
} vlc_vk_instance_t;

#endif // VLC_VULKAN_INSTANCE_H
