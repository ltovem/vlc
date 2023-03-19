// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * renderer.h: OpenGL internal header
 *****************************************************************************
 * Copyright (C) 2016 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef VLC_GL_RENDERER_H
#define VLC_GL_RENDERER_H

#include <vlc_common.h>
#include <vlc_codec.h>
#include <vlc_opengl.h>
#include <vlc_plugin.h>

#include "filter.h"
#include "gl_api.h"
#include "gl_common.h"
#include "interop.h"
#include "sampler.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define add_opengl_submodule_renderer() \
    add_submodule() \
    add_shortcut("renderer") \
    set_shortname("renderer") \
    set_capability("opengl filter", 0) \
    set_callback_opengl_filter(vlc_gl_renderer_Open)

/**
 * OpenGL picture renderer
 */
struct vlc_gl_renderer
{
    /* Set by the caller */
    const struct vlc_gl_api *api;
    const opengl_vtable_t *vt; /* for convenience, same as &api->vt */

    /* True to dump shaders */
    bool dump_shaders;

    GLuint program_id;

    struct {
        GLfloat ProjectionMatrix[16];
        GLfloat StereoMatrix[3*3];
        GLfloat ZoomMatrix[16];
        GLfloat ViewMatrix[16];
    } var;

    struct {
        GLint StereoMatrix;
        GLint ProjectionMatrix;
        GLint ViewMatrix;
        GLint ZoomMatrix;
    } uloc;

    struct {
        GLint PicCoordsIn;
        GLint VertexPosition;
    } aloc;

    struct vlc_gl_sampler *sampler;

    unsigned nb_indices;
    GLuint vertex_buffer_object;
    GLuint index_buffer_object;
    GLuint texture_buffer_object;

    bool valid_coords;

    /* View point */
    vlc_viewpoint_t vp;
    float f_teta;
    float f_phi;
    float f_roll;
    float f_fovx; /* f_fovx and f_fovy are linked but we keep both */
    float f_fovy; /* to avoid recalculating them when needed.      */
    float f_z;    /* Position of the camera on the sphere radius vector */
    float f_sar;

    /* Original size from which f_sar is computed
     *     f_sar = (float) target_width / target_height */
    unsigned target_width;
    unsigned target_height;
};

vlc_gl_filter_open_fn vlc_gl_renderer_Open;

int
vlc_gl_renderer_SetViewpoint(struct vlc_gl_renderer *renderer,
                             const vlc_viewpoint_t *p_vp);

#ifdef __cplusplus
}
#endif

#endif /* include-guard */
