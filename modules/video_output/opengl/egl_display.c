// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * egl_display.c
 *****************************************************************************
 * Copyright (C) 2021 Videolabs
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>
#include <vlc_common.h>
#include <vlc_modules.h>

#include "egl_display.h"

struct vlc_egl_display *
vlc_egl_display_New(vlc_object_t *parent, const char *name)
{
    struct vlc_egl_display *display =
        vlc_object_create(parent, sizeof(*display));
    if (!display)
        return NULL;

    display->ops = NULL;
    display->display = EGL_NO_DISPLAY;

    if (!name || !*name)
        name = "any";

    module_t **mods;
    ssize_t total = vlc_module_match("egl display", name, true, &mods, NULL);
    for (ssize_t i = 0; i < total; ++i)
    {
        vlc_egl_display_open_fn *open = vlc_module_map(parent->logger, mods[i]);

        if (open && open(display) == VLC_SUCCESS)
        {
            assert(display->display != EGL_NO_DISPLAY);
            free(mods);
            return display;
        }
    }

    free(mods);
    vlc_object_delete(display);
    return NULL;
}

void vlc_egl_display_Delete(struct vlc_egl_display *display)
{
    if (display->ops && display->ops->close) {
        display->ops->close(display);
    }

    vlc_object_delete(display);
}
