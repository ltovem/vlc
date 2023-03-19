// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * egl_display.h
 *****************************************************************************
 * Copyright (C) 2021 Videolabs
 *****************************************************************************/

#ifndef VLC_EGL_REFDISPLAY_H
#define VLC_EGL_REFDISPLAY_H

#include <vlc_common.h>
#include <EGL/egl.h>

/**
 * Structure for module "egl display", to open an EGL display guaranteed to
 * be internally refcounted.
 */
struct vlc_egl_display
{
    struct vlc_object_t obj;

    EGLDisplay display;

    const struct vlc_egl_display_ops *ops;
    void *sys;
};

struct vlc_egl_display_ops {
    void (*close)(struct vlc_egl_display *display);
};

typedef int
vlc_egl_display_open_fn(struct vlc_egl_display *display);

struct vlc_egl_display *
vlc_egl_display_New(vlc_object_t *parent, const char *name);

void vlc_egl_display_Delete(struct vlc_egl_display *display);

#endif
