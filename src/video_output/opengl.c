/*****************************************************************************
 * opengl.c: VLC GL API
 *****************************************************************************
 * Copyright (C) 2011 Rémi Denis-Courmont
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <assert.h>
#include <stdlib.h>

#include <vlc_common.h>
#include <vlc_atomic.h>
#include <vlc_opengl.h>
#include <vlc_codec.h>
#include <vlc_vout_display.h>
#include "libvlc.h"
#include <vlc_modules.h>

struct vlc_gl_priv_t
{
    vlc_gl_t gl;
    vlc_atomic_rc_t rc;

    struct {
        const uint8_t* (*GetString)(uint32_t);
        const uint8_t* (*GetStringi)(uint32_t, uint32_t);
        void (*GetIntegerv)(uint32_t, int32_t *);
        uint32_t (*GetError)();
    } vt;
};

static int vlc_gl_start(void *func, bool forced, va_list ap)
{
    int (*activate)(vlc_gl_t *, unsigned, unsigned) = func;
    vlc_gl_t *gl = va_arg(ap, vlc_gl_t *);
    unsigned width = va_arg(ap, unsigned);
    unsigned height = va_arg(ap, unsigned);

    int ret = activate(gl, width, height);
    if (ret)
        vlc_objres_clear(VLC_OBJECT(gl));
    (void) forced;
    return ret;
}

vlc_gl_t *(vlc_gl_Create)(
        const struct vout_display_cfg *restrict cfg,
        unsigned flags, const char *name,
        const struct vlc_gl_callbacks *cbs, void *owner)
{
    vout_window_t *wnd = cfg->window;
    struct vlc_gl_priv_t *glpriv;
    const char *type;

    enum vlc_gl_api_type api_type;

    switch (flags /*& VLC_OPENGL_API_MASK*/)
    {
        case VLC_OPENGL:
            type = "opengl";
            api_type = VLC_OPENGL;
            break;
        case VLC_OPENGL_ES2:
            type = "opengl es2";
            api_type = VLC_OPENGL_ES2;
            break;
        default:
            return NULL;
    }

    glpriv = vlc_custom_create(VLC_OBJECT(wnd), sizeof (*glpriv), "gl");
    if (unlikely(glpriv == NULL))
        return NULL;

    vlc_gl_t *gl = &glpriv->gl;
    gl->api_type = api_type;
    gl->surface = wnd;
    gl->device = NULL;
    gl->owner.cbs = cbs;
    gl->owner.sys = owner;
    glpriv->vt.GetString = NULL;
    glpriv->vt.GetStringi = NULL;
    glpriv->vt.GetIntegerv = NULL;

    gl->module = vlc_module_load(gl, type, name, true, vlc_gl_start, gl,
                                 cfg->display.width, cfg->display.height);
    if (gl->module == NULL)
    {
        vlc_object_delete(gl);
        return NULL;
    }
    assert(gl->ops);
    assert(gl->ops->make_current);
    assert(gl->ops->release_current);
    assert(gl->ops->swap);
    assert(gl->ops->get_proc_address);
    vlc_atomic_rc_init(&glpriv->rc);

    return &glpriv->gl;
}

vlc_gl_t *vlc_gl_CreateOffscreen(vlc_object_t *parent,
                                 struct vlc_decoder_device *device,
                                 unsigned width, unsigned height,
                                 unsigned flags, const char *name)
{
    struct vlc_gl_priv_t *glpriv;
    const char *type;

    enum vlc_gl_api_type api_type;

    switch (flags /*& VLC_OPENGL_API_MASK*/)
    {
        case VLC_OPENGL:
            type = "opengl offscreen";
            api_type = VLC_OPENGL;
            break;
        case VLC_OPENGL_ES2:
            type = "opengl es2 offscreen";
            api_type = VLC_OPENGL_ES2;
            break;
        default:
            return NULL;
    }

    glpriv = vlc_custom_create(parent, sizeof (*glpriv), "gl");
    if (unlikely(glpriv == NULL))
        return NULL;

    vlc_gl_t *gl = &glpriv->gl;

    gl->api_type = api_type;

    gl->offscreen_chroma_out = VLC_CODEC_UNKNOWN;
    gl->offscreen_vflip = false;
    gl->offscreen_vctx_out = NULL;

    gl->surface = NULL;
    gl->device = device ? vlc_decoder_device_Hold(device) : NULL;
    gl->module = vlc_module_load(gl, type, name, true, vlc_gl_start, gl, width,
                                 height);
    if (gl->module == NULL)
    {
        vlc_object_delete(gl);
        return NULL;
    }

    /* The implementation must initialize the output chroma */
    assert(gl->offscreen_chroma_out != VLC_CODEC_UNKNOWN);

    vlc_atomic_rc_init(&glpriv->rc);

    assert(gl->ops->make_current);
    assert(gl->ops->release_current);
    assert(gl->ops->swap_offscreen);
    assert(gl->ops->get_proc_address);

    return &glpriv->gl;
}

void vlc_gl_Hold(vlc_gl_t *gl)
{
    struct vlc_gl_priv_t *glpriv = (struct vlc_gl_priv_t *)gl;
    vlc_atomic_rc_inc(&glpriv->rc);
}

void vlc_gl_Release(vlc_gl_t *gl)
{
    struct vlc_gl_priv_t *glpriv = (struct vlc_gl_priv_t *)gl;
    if (!vlc_atomic_rc_dec(&glpriv->rc))
        return;

    if (gl->ops->close != NULL)
        gl->ops->close(gl);

    if (gl->device)
        vlc_decoder_device_Release(gl->device);

    vlc_objres_clear(VLC_OBJECT(gl));
    vlc_object_delete(gl);
}

bool vlc_gl_HasExtension(vlc_gl_t *gl, const char *extension)
{
#define GL_NO_ERROR 0
#define GL_VERSION 0x1F02
#define GL_MAJOR_VERSION 0x821B
#define GL_EXTENSIONS 0x1F03
#define GL_NUM_EXTENSIONS 0x821D
    struct vlc_gl_priv_t *glpriv = (struct vlc_gl_priv_t *)gl;
    /* Cache the OpenGL function before checking. It's not done at OpenGL
     * provider creation because the context might not be current, and it is
     * not done at MakeCurrent because MakeCurrent is currently called at each
     * frames, whereas vlc_gl_HasExtension can be called only during the
     * client code initialization. */
    if (glpriv->vt.GetString == NULL && glpriv->vt.GetStringi == NULL)
    {
        glpriv->vt.GetString = vlc_gl_GetProcAddress(gl, "glGetString");
        glpriv->vt.GetIntegerv = vlc_gl_GetProcAddress(gl, "glGetIntegerv");
        glpriv->vt.GetError = vlc_gl_GetProcAddress(gl, "glGetError");

        int32_t version;
        /* GL_MAJOR_VERSION is available in every OpenGL and GLES>=3.
         * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGet.xhtml
         * https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glGet.xhtml
         * It will return a GL_INVALID_ENUM error on GLES<3
         */
        glpriv->vt.GetIntegerv(GL_MAJOR_VERSION, &version);
        uint32_t error = glpriv->vt.GetError();

        if (error != GL_NO_ERROR)
            version = 2;

        /* Drain the errors before continuing. */
        while (error != GL_NO_ERROR)
            error = glpriv->vt.GetError();

        /* glGetStringi is available in OpenGL>=3 and GLES>=3.
         * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetString.xhtml
         * https://www.khronos.org/registry/OpenGL-Refpages/es3/html/glGetString.xhtml
         */
        if (version >= 3)
            glpriv->vt.GetStringi = vlc_gl_GetProcAddress(gl, "glGetStringi");
    }

    /* Fallback to legacy checking mode. */
    if (glpriv->vt.GetStringi == NULL)
    {
        const uint8_t *extensions = glpriv->vt.GetString(GL_EXTENSIONS);
        return vlc_gl_StrHasToken((const char *)extensions, extension);
    }

    /* Unfortunately, no order is defined by the standard, so just loop over
     * the different extensions linearily. */
    int32_t count = 0;
    glpriv->vt.GetIntegerv(GL_NUM_EXTENSIONS, &count);
    for (int i = 0; i < count; ++i)
    {
        const uint8_t *name = glpriv->vt.GetStringi(GL_EXTENSIONS, i);
        if (strcmp((const char *)name, extension) == 0)
            return true;
    }
    return false;
}

#include <vlc_vout_window.h>

typedef struct vlc_gl_surface
{
    int width;
    int height;
    vlc_mutex_t lock;
} vlc_gl_surface_t;

static void vlc_gl_surface_ResizeNotify(vout_window_t *surface,
                                        unsigned width, unsigned height,
                                        vout_window_ack_cb cb, void *opaque)
{
    vlc_gl_surface_t *sys = surface->owner.sys;

    msg_Dbg(surface, "resized to %ux%u", width, height);

    vlc_mutex_lock(&sys->lock);
    sys->width = width;
    sys->height = height;

    if (cb != NULL)
        cb(surface, width, height, opaque);
    vlc_mutex_unlock(&sys->lock);
}

vlc_gl_t *(vlc_gl_surface_Create)(
        vlc_object_t *obj, const vout_window_cfg_t *cfg,
        const struct vlc_gl_callbacks *gl_cbs, void *gl_owner,
        struct vout_window_t **restrict wp)
{
    vlc_gl_surface_t *sys = malloc(sizeof (*sys));
    if (unlikely(sys == NULL))
        return NULL;

    sys->width = cfg->width;
    sys->height = cfg->height;
    vlc_mutex_init(&sys->lock);

    static const struct vout_window_callbacks cbs = {
        .resized = vlc_gl_surface_ResizeNotify,
    };
    vout_window_owner_t owner = {
        .cbs = &cbs,
        .sys = sys,
    };
    char *modlist = var_InheritString(obj, "window");

    vout_window_t *surface = vout_window_New(obj, modlist, &owner, cfg);
    free(modlist);
    if (surface == NULL)
        goto error;
    if (vout_window_Enable(surface)) {
        vout_window_Delete(surface);
        goto error;
    }
    if (wp != NULL)
        *wp = surface;

    /* TODO: support ES? */
    struct vout_display_cfg dcfg = {
        .window = surface,
        .display = { .width = cfg->width, cfg->height },
    };

    vlc_mutex_lock(&sys->lock);
    if (sys->width >= 0 && sys->height >= 0) {
        dcfg.display.width = sys->width;
        dcfg.display.height = sys->height;
        sys->width = -1;
        sys->height = -1;
    }
    vlc_mutex_unlock(&sys->lock);

    vlc_gl_t *gl = vlc_gl_CreateWithOwner(&dcfg, VLC_OPENGL,
            NULL, gl_cbs, gl_owner);
    if (gl == NULL) {
        vout_window_Disable(surface);
        vout_window_Delete(surface);
        goto error;
    }

    return gl;

error:
    free(sys);
    return NULL;
}

/**
 * Checks if the dimensions of the surface used by the OpenGL context have
 * changed (since the previous call), and  the OpenGL viewport should be
 * updated.
 * \return true if at least one dimension has changed, false otherwise
 * \warning This function is intrinsically race-prone.
 * The dimensions can change asynchronously.
 */
bool vlc_gl_surface_CheckSize(vlc_gl_t *gl, unsigned *restrict width,
                              unsigned *restrict height)
{
    vout_window_t *surface = gl->surface;
    vlc_gl_surface_t *sys = surface->owner.sys;
    bool ret = false;

    vlc_mutex_lock(&sys->lock);
    if (sys->width >= 0 && sys->height >= 0)
    {
        *width = sys->width;
        *height = sys->height;
        sys->width = -1;
        sys->height = -1;

        vlc_gl_Resize(gl, *width, *height);
        ret = true;
    }
    vlc_mutex_unlock(&sys->lock);
    return ret;
}

void vlc_gl_surface_Destroy(vlc_gl_t *gl)
{
    vout_window_t *surface = gl->surface;
    vlc_gl_surface_t *sys = surface->owner.sys;

    vlc_gl_Release(gl);
    vout_window_Disable(surface);
    vout_window_Delete(surface);
    free(sys);
}
