// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * egl_display_generic.c
 *****************************************************************************
 * Copyright (C) 2021 Videolabs
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_opengl.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "egl_display.h"

#ifndef __ANDROID__
static EGLenum GetPlatform(const char *extensions)
{
#ifdef EGL_KHR_platform_x11
    if (vlc_gl_StrHasToken(extensions, "EGL_EXT_platform_x11"))
        return EGL_PLATFORM_X11_KHR;
#endif

#ifdef EGL_KHR_platform_wayland
    if (vlc_gl_StrHasToken(extensions, "EGL_EXT_platform_wayland"))
        return EGL_PLATFORM_WAYLAND_KHR;
#endif

    return 0;
}
#endif

static vlc_egl_display_open_fn Open;
static int
Open(struct vlc_egl_display *display)
{
#ifdef __ANDROID__
    /* The default display is refcounted on Android */
    display->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#elif defined(EGL_KHR_display_reference)
    const char *extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);

    if (!vlc_gl_StrHasToken(extensions, "EGL_KHR_display_reference"))
        return VLC_EGENERIC;

    EGLenum platform = GetPlatform(extensions);
    if (!platform)
        return VLC_EGENERIC;

    const EGLAttrib attribs[] = {
        EGL_TRACK_REFERENCES_KHR, EGL_TRUE,
        EGL_NONE,
    };

    display->display =
        eglGetPlatformDisplay(platform, EGL_DEFAULT_DISPLAY, attribs);
#endif

    if (display->display == EGL_NO_DISPLAY)
        return VLC_EGENERIC;

    return VLC_SUCCESS;
}

vlc_module_begin()
    set_description("EGL generic display")
    set_capability("egl display", 1)
    set_callback(Open)
    add_shortcut("egl_display_generic")
vlc_module_end()
