/*****************************************************************************
 * platform.c: Vulkan platform abstraction
 *****************************************************************************
 * Copyright (C) 2018 Niklas Haas
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#include <assert.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_modules.h>

#include "platform.h"

static int vlc_vk_start(void *func, bool forced, va_list ap)
{
    int (*activate)(vlc_vk_platform_t *vk) = func;
    vlc_vk_platform_t *vk = va_arg(ap, vlc_vk_platform_t *);

    int ret = activate(vk);
    /* TODO: vlc_objres_clear, which is not in the public API. */
    (void)forced;
    return ret;
}

/**
 * Initializes a Vulkan platform module for a given window
 *
 * @param wnd window to use as Vulkan surface
 * @param name module name (or "any")
 * @return a new platform object, or NULL on failure
 */
vlc_vk_platform_t *vlc_vk_platform_Create(struct vlc_window *wnd, const char *name)
{
    vlc_object_t *parent = VLC_OBJECT(wnd);
    struct vlc_vk_platform_t *vk;

    vk = vlc_object_create(parent, sizeof (*vk));
    if (unlikely(vk == NULL))
        return NULL;

    vk->platform_ext = NULL;
    vk->ops = NULL;
    vk->window = wnd;

    vk->module = vlc_module_load(wnd, "vulkan platform", name, true,
                                 vlc_vk_start, vk);

    if (vk->module == NULL)
    {
        vlc_object_delete(vk);
        return NULL;
    }

    return vk;
}

void vlc_vk_platform_Release(vlc_vk_platform_t *vk)
{
    if (vk->ops)
        vk->ops->close(vk);

    /* TODO: use vlc_objres_clear */
    vlc_object_delete(vk);
}
