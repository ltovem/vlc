// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * interop.h
 *****************************************************************************
 * Copyright (C) 2019 Videolabs
 *****************************************************************************/

#ifndef VLC_GL_INTEROP_PRIV_H
#define VLC_GL_INTEROP_PRIV_H

#include <vlc_common.h>
#include <vlc_opengl.h>
#include <vlc_picture.h>
#include <vlc_picture_pool.h>
#include <vlc_opengl_interop.h>
#include "gl_common.h"


struct vlc_gl_interop *
vlc_gl_interop_New(struct vlc_gl_t *gl, vlc_video_context *context,
                   const video_format_t *fmt);

struct vlc_gl_interop *
vlc_gl_interop_NewForSubpictures(struct vlc_gl_t *gl);

void
vlc_gl_interop_Delete(struct vlc_gl_interop *interop);

int
vlc_gl_interop_GenerateTextures(const struct vlc_gl_interop *interop,
                                const GLsizei *tex_width,
                                const GLsizei *tex_height, GLuint *textures);

void
vlc_gl_interop_DeleteTextures(const struct vlc_gl_interop *interop,
                              GLuint *textures);

#endif
