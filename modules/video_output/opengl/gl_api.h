// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * gl_api.h
 *****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 * Copyright (C) 2020 Videolabs
 *****************************************************************************/

#ifndef VLC_GL_API_H
#define VLC_GL_API_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdbool.h>
#include <vlc_common.h>
#include <vlc_opengl.h>

#include "gl_common.h"

struct vlc_gl_api {
    opengl_vtable_t vt;

    /* True if the current API is OpenGL ES, set by the caller */
    bool is_gles;

    /* Non-power-of-2 texture size support */
    bool supports_npot;

    /* Multisampling for anti-aliasing */
    bool supports_multisample;
};

int
vlc_gl_api_Init(struct vlc_gl_api *api, vlc_gl_t *gl);

#endif
