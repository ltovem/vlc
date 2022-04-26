/**
 * @file egl.c
 * @brief EGL OpenGL extension module
 */
/*****************************************************************************
 * Copyright © 2010-2014 Rémi Denis-Courmont
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

#include <stdlib.h>
#include <assert.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_opengl.h>
#include <vlc_vout_window.h>
#ifdef USE_PLATFORM_X11
# include <vlc_xlib.h>
#endif
#ifdef USE_PLATFORM_XCB
# include <xcb/xcb.h>
# include "../xcb/events.h"
#endif
#ifdef USE_PLATFORM_WAYLAND
# include <wayland-egl.h>
#endif
#if defined (USE_PLATFORM_ANDROID)
# include "../android/utils.h"
#endif

typedef struct vlc_gl_sys_t
{
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
#if defined (USE_PLATFORM_X11)
    Display *x11;
    bool restore_forget_gravity;
#endif
#if defined (USE_PLATFORM_XCB)
    xcb_connection_t *conn;
    bool restore_forget_gravity;
#endif
#if defined (USE_PLATFORM_WAYLAND)
    struct wl_egl_window *window;
#endif
} vlc_gl_sys_t;

static int MakeCurrent (vlc_gl_t *gl)
{
    vlc_gl_sys_t *sys = gl->sys;

    if (eglMakeCurrent (sys->display, sys->surface, sys->surface,
                        sys->context) != EGL_TRUE)
        return VLC_EGENERIC;
    return VLC_SUCCESS;
}

static void ReleaseCurrent (vlc_gl_t *gl)
{
    vlc_gl_sys_t *sys = gl->sys;

    eglMakeCurrent (sys->display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                    EGL_NO_CONTEXT);
}

#ifdef USE_PLATFORM_XCB
static int GetScreenNum(xcb_connection_t *conn, const xcb_screen_t *scr)
{
    xcb_screen_iterator_t i = xcb_setup_roots_iterator(xcb_get_setup(conn));
    int n = 0;

    while (scr != i.data) {
         xcb_screen_next(&i);
         n++;
    }
    return n;
}
#endif

#ifdef USE_PLATFORM_WAYLAND
static void Resize (vlc_gl_t *gl, unsigned width, unsigned height)
{
    vlc_gl_sys_t *sys = gl->sys;

    wl_egl_window_resize(sys->window, width, height, 0, 0);
}
#else
# define Resize (NULL)
#endif

static void SwapBuffers (vlc_gl_t *gl)
{
    vlc_gl_sys_t *sys = gl->sys;

    eglSwapBuffers (sys->display, sys->surface);
}

static void *GetSymbol(vlc_gl_t *gl, const char *procname)
{
    (void) gl;
    return (void *)eglGetProcAddress (procname);
}

static bool CheckAPI (EGLDisplay dpy, const char *api)
{
    const char *apis = eglQueryString (dpy, EGL_CLIENT_APIS);
    return vlc_gl_StrHasToken(apis, api);
}

static bool CheckClientExt(const char *name)
{
    const char *exts = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
    return vlc_gl_StrHasToken(exts, name);
}

static bool CheckDisplayExt(EGLDisplay display, const char *name)
{
    const char *exts = eglQueryString(display, EGL_EXTENSIONS);
    return vlc_gl_StrHasToken(exts, name);
}

static void DisplayAvailableDevices(vlc_gl_t *gl)
{
    PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT =
        (PFNEGLQUERYDEVICESEXTPROC)eglGetProcAddress("eglQueryDevicesEXT");
    PFNEGLQUERYDEVICESTRINGEXTPROC eglQueryDeviceStringEXT =
        (PFNEGLQUERYDEVICESTRINGEXTPROC)eglGetProcAddress("eglQueryDeviceStringEXT");

    bool has_device_query_name = CheckClientExt("EGL_EXT_device_query_name");

    EGLint device_count = 0;
    eglQueryDevicesEXT(0, NULL, &device_count);

    EGLDeviceEXT *devices = malloc(sizeof(*devices) * device_count);
    eglQueryDevicesEXT(device_count, devices, &device_count);
    msg_Dbg(gl, "EGL has %d devices:", device_count);

    for (EGLint i=0; i<device_count; ++i)
    {
        const char *vendor;
        const char *renderer;
        const char *device_ext =
            eglQueryDeviceStringEXT(devices[i], EGL_EXTENSIONS);

        if (has_device_query_name)
        {
            vendor = eglQueryDeviceStringEXT(devices[i], EGL_VENDOR);
            renderer = eglQueryDeviceStringEXT(devices[i], EGL_RENDERER_EXT);
            msg_Dbg(gl, " - device %d: vendor='%s' renderer='%s'",
                    i, vendor, renderer);
        }
        else
            msg_Dbg(gl, " - device %d: EGL_EXT_device_query_name not available", i);

        const char *space = device_ext;
        while (*space != '\0')
        {
            space = strchr(device_ext, ' ');
            if (space == NULL)
                space = device_ext + strlen(device_ext);
            size_t length = space - device_ext;

            const char *additional_info = NULL;

#define DEVICE_EXTENSION_MATCH(ext) \
            (sizeof(ext) - 1 == length && strncmp(device_ext, ext, length) == 0)

            if (DEVICE_EXTENSION_MATCH("EGL_EXT_device_drm_render_node"))
                additional_info = eglQueryDeviceStringEXT(devices[i], EGL_DRM_RENDER_NODE_FILE_EXT);
            else if (DEVICE_EXTENSION_MATCH("EGL_EXT_device_drm"))
                additional_info = eglQueryDeviceStringEXT(devices[i], EGL_DRM_DEVICE_FILE_EXT);

            msg_Dbg(gl, "   + extension: type='%.*s'%s%s", (int)length, device_ext,
                    additional_info ? ": " : "",
                    additional_info ? additional_info : "");
            device_ext = space + 1;
        }
    }

    free(devices);
}

struct gl_api
{
   const char name[10];
   EGLenum    api;
   EGLint     min_minor;
   EGLint     render_bit;
   EGLint     attr[3];
};

#ifdef EGL_EXT_platform_base
static EGLDisplay GetDisplayEXT(EGLenum plat, void *dpy, const EGLint *attrs)
{
    PFNEGLGETPLATFORMDISPLAYEXTPROC getDisplay =
        (PFNEGLGETPLATFORMDISPLAYEXTPROC)
        eglGetProcAddress("eglGetPlatformDisplayEXT");

    assert(getDisplay != NULL);
    return getDisplay(plat, dpy, attrs);
}

static EGLSurface CreateWindowSurfaceEXT(EGLDisplay dpy, EGLConfig config,
                                         void *window, const EGLint *attrs)
{
    PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC createSurface =
        (PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC)
        eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT");

    assert(createSurface != NULL);
    return createSurface(dpy, config, window, attrs);
}
#endif

static EGLSurface CreateWindowSurface(EGLDisplay dpy, EGLConfig config,
                                      void *window, const EGLint *attrs)
{
    EGLNativeWindowType *native = window;

    return eglCreateWindowSurface(dpy, config, *native, attrs);
}

static void Close(vlc_gl_t *gl)
{
    vlc_gl_sys_t *sys = gl->sys;

    if (sys->display != EGL_NO_DISPLAY)
    {
        if (sys->context != EGL_NO_CONTEXT)
            eglDestroyContext(sys->display, sys->context);
        if (sys->surface != EGL_NO_SURFACE)
            eglDestroySurface(sys->display, sys->surface);
        eglTerminate(sys->display);
    }
#ifdef USE_PLATFORM_X11
    if (sys->x11 != NULL) {
        if (sys->restore_forget_gravity) {
            vout_window_t *wnd = gl->surface;
            XSetWindowAttributes swa;
            swa.bit_gravity = ForgetGravity;
            XChangeWindowAttributes(sys->x11, wnd->handle.xid, CWBitGravity,
                                    &swa);
        }
        XCloseDisplay(sys->x11);
    }
#elif defined (USE_PLATFORM_XCB)
    if (sys->conn != NULL) {
        if (sys->restore_forget_gravity) {
            const uint32_t values[] = { XCB_GRAVITY_BIT_FORGET };

            xcb_change_window_attributes(sys->conn, gl->surface->handle.xid,
                                         XCB_CW_BIT_GRAVITY, values);
        }
        xcb_disconnect(sys->conn);
    }
#endif
#ifdef USE_PLATFORM_WAYLAND
    if (sys->window != NULL)
        wl_egl_window_destroy(sys->window);
#endif
#ifdef USE_PLATFORM_ANDROID
    AWindowHandler_releaseANativeWindow(gl->surface->handle.anativewindow,
                                        AWindow_Video);
#endif
    free (sys);
}

/**
 * Probe EGL display availability
 */
static int Open(vlc_gl_t *gl, const struct gl_api *api,
                unsigned width, unsigned height)
{
    int ret = VLC_EGENERIC;
    vlc_object_t *obj = VLC_OBJECT(gl);
    vlc_gl_sys_t *sys = malloc(sizeof (*sys));
    if (unlikely(sys == NULL))
        return VLC_ENOMEM;

    gl->sys = sys;
    sys->display = EGL_NO_DISPLAY;
    sys->surface = EGL_NO_SURFACE;
    sys->context = EGL_NO_CONTEXT;

    vout_window_t *wnd = gl->surface;
    EGLSurface (*createSurface)(EGLDisplay, EGLConfig, void *, const EGLint *)
        = CreateWindowSurface;
    void *window;
    EGLAttrib refs_name = EGL_NONE;
    EGLAttrib refs_value = EGL_FALSE;

#ifdef EGL_KHR_display_reference
    if (CheckClientExt("EGL_KHR_display_reference"))
    {
        refs_name = EGL_TRACK_REFERENCES_KHR;
        refs_value = EGL_TRUE;
    }
#endif

#ifdef USE_PLATFORM_X11
    sys->x11 = NULL;

    if (wnd->type != VOUT_WINDOW_TYPE_XID || !vlc_xlib_init(obj))
        goto error;

    window = &wnd->handle.xid;
    sys->x11 = XOpenDisplay(wnd->display.x11);
    if (sys->x11 == NULL)
        goto error;

    int snum;
    {
        XWindowAttributes wa;
        XSetWindowAttributes swa;

        sys->restore_forget_gravity = false;
        if (!XGetWindowAttributes(sys->x11, wnd->handle.xid, &wa))
            goto error;
        snum = XScreenNumberOfScreen(wa.screen);
        if (wa.bit_gravity == ForgetGravity) {
            swa.bit_gravity = NorthWestGravity;
            XChangeWindowAttributes(sys->x11, wnd->handle.xid, CWBitGravity,
                                    &swa);
            sys->restore_forget_gravity = true;
        }
    }
# ifdef EGL_EXT_platform_x11
    if (CheckClientExt("EGL_EXT_platform_x11"))
    {
        const EGLint attrs[] = {
            EGL_PLATFORM_X11_SCREEN_EXT, snum,
            EGL_NONE
        };
        sys->display = GetDisplayEXT(EGL_PLATFORM_X11_EXT, sys->x11, attrs);
        createSurface = CreateWindowSurfaceEXT;
    }
    else
# endif
    {
# ifdef __unix__
        if (snum == XDefaultScreen(sys->x11))
            sys->display = eglGetDisplay(sys->x11);
# endif
    }
    (void) width; (void) height;

#elif defined (USE_PLATFORM_XCB)
    xcb_connection_t *conn;
    const xcb_screen_t *scr;

    sys->conn = NULL;
    sys->restore_forget_gravity = false;

    if (wnd->type != VOUT_WINDOW_TYPE_XID)
        goto error;

# ifdef EGL_EXT_platform_xcb
    if (!CheckClientExt("EGL_EXT_platform_xcb"))
        goto error;

    ret = vlc_xcb_parent_Create(gl->obj.logger, wnd, &conn, &scr);
    if (ret == VLC_SUCCESS)
    {
        xcb_get_window_attributes_reply_t *r =
            xcb_get_window_attributes_reply(conn,
                xcb_get_window_attributes(conn, wnd->handle.xid), NULL);

        if (r != NULL && r->bit_gravity == XCB_GRAVITY_BIT_FORGET) {
            const uint32_t values[] = { XCB_GRAVITY_NORTH_WEST };

            xcb_change_window_attributes(sys->conn, gl->surface->handle.xid,
                                         XCB_CW_BIT_GRAVITY, values);
            sys->restore_forget_gravity = true;
        }
        free(r);
    }
    else
        goto error;

    sys->conn = conn;
    window = &wnd->handle.xid;

    {
        const EGLint attrs[] = {
            EGL_PLATFORM_XCB_SCREEN_EXT, GetScreenNum(sys->conn, scr),
            EGL_NONE
        };
        sys->display = GetDisplayEXT(EGL_PLATFORM_XCB_EXT, sys->conn, attrs);
        createSurface = CreateWindowSurfaceEXT;
    }
# endif
    (void) width; (void) height;

#elif defined (USE_PLATFORM_WAYLAND)
    sys->window = NULL;

    if (wnd->type != VOUT_WINDOW_TYPE_WAYLAND)
        goto error;

# ifdef EGL_EXT_platform_wayland
    if (!CheckClientExt("EGL_EXT_platform_wayland"))
        goto error;

    window = wl_egl_window_create(wnd->handle.wl, width, height);
    if (window == NULL)
        goto error;
    sys->window = window;

    sys->display = GetDisplayEXT(EGL_PLATFORM_WAYLAND_EXT, wnd->display.wl,
                                 NULL);
    createSurface = CreateWindowSurfaceEXT;

# endif

#elif defined (USE_PLATFORM_WIN32)
    if (wnd->type != VOUT_WINDOW_TYPE_HWND)
        goto error;

    window = &wnd->handle.hwnd;
# if defined (_WIN32) || defined (__VC32__) \
  && !defined (__CYGWIN__) && !defined (__SCITECH_SNAP__)
    sys->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
# endif
    (void) width; (void) height;

#elif defined (USE_PLATFORM_ANDROID)
    if (wnd->type != VOUT_WINDOW_TYPE_ANDROID_NATIVE)
        goto error;

    ANativeWindow *anw =
        AWindowHandler_getANativeWindow(wnd->handle.anativewindow,
                                        AWindow_Video);
    if (anw == NULL)
        goto error;
    window = &anw;
# if defined (__ANDROID__) || defined (ANDROID)
    sys->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
# endif
    (void) width; (void) height;

#endif

    if (sys->display == EGL_NO_DISPLAY)
        goto error;

    /* Initialize EGL display */
    EGLint major, minor;
    if (eglInitialize(sys->display, &major, &minor) != EGL_TRUE)
        goto error;
    msg_Dbg(obj, "EGL version %s by %s",
            eglQueryString(sys->display, EGL_VERSION),
            eglQueryString(sys->display, EGL_VENDOR));

    const char *ext = eglQueryString(sys->display, EGL_EXTENSIONS);
    if (*ext)
        msg_Dbg(obj, " extensions: %s", ext);

    if (major != 1 || minor < api->min_minor
     || !CheckAPI(sys->display, api->name))
    {
        msg_Err(obj, "cannot select %s API", api->name);
        goto error;
    }

    bool has_device_enum = CheckClientExt("EGL_EXT_device_enumeration");
    bool has_device_query = CheckClientExt("EGL_EXT_device_query");

    if (has_device_query && has_device_enum)
    {
        DisplayAvailableDevices(gl);
        PFNEGLQUERYDISPLAYATTRIBEXTPROC eglQueryDisplayAttribEXT =
            (PFNEGLQUERYDISPLAYATTRIBEXTPROC)eglGetProcAddress("eglQueryDisplayAttribEXT");
        PFNEGLQUERYDEVICESTRINGEXTPROC eglQueryDeviceStringEXT =
            (PFNEGLQUERYDEVICESTRINGEXTPROC)eglGetProcAddress("eglQueryDeviceStringEXT");

        EGLAttrib device_attr;
        eglQueryDisplayAttribEXT(sys->display, EGL_DEVICE_EXT, &device_attr);
        EGLDeviceEXT device = (EGLDeviceEXT)device_attr;

        const char *extensions =
            eglQueryDeviceStringEXT(device, EGL_EXTENSIONS);

        msg_Dbg(gl, "Using EGL device: %s", extensions);
    }

    bool has_query_driver =
        CheckDisplayExt(sys->display, "EGL_MESA_query_driver");

    if (has_query_driver)
    {
        PFNEGLGETDISPLAYDRIVERNAMEPROC eglGetDisplayDriverName =
            (PFNEGLGETDISPLAYDRIVERNAMEPROC) eglGetProcAddress("eglGetDisplayDriverName");

        msg_Dbg(gl, "Using EGL driver: %s", eglGetDisplayDriverName(sys->display));
    }
    else msg_Dbg(gl, "EGL_MESA_query_driver is unavailable");


    const EGLint conf_attr[] = {
        EGL_RED_SIZE, 5,
        EGL_GREEN_SIZE, 5,
        EGL_BLUE_SIZE, 5,
        EGL_RENDERABLE_TYPE, api->render_bit,
        refs_name, refs_value,
        EGL_NONE
    };
    EGLConfig cfgv[1];
    EGLint cfgc;

    if (eglChooseConfig(sys->display, conf_attr, cfgv, 1, &cfgc) != EGL_TRUE
     || cfgc == 0)
    {
        msg_Err (obj, "cannot choose EGL configuration");
        goto error;
    }

    /* Create a drawing surface */
    sys->surface = createSurface(sys->display, cfgv[0], window, NULL);
    if (sys->surface == EGL_NO_SURFACE)
    {
        msg_Err (obj, "cannot create EGL window surface");
        goto error;
    }

    if (eglBindAPI (api->api) != EGL_TRUE)
    {
        msg_Err (obj, "cannot bind EGL API");
        goto error;
    }

    EGLContext ctx = eglCreateContext(sys->display, cfgv[0], EGL_NO_CONTEXT,
                                      api->attr);
    if (ctx == EGL_NO_CONTEXT)
    {
        msg_Err (obj, "cannot create EGL context");
        goto error;
    }
    sys->context = ctx;

    /* Initialize OpenGL callbacks */
    gl->make_current = MakeCurrent;
    gl->release_current = ReleaseCurrent;
    gl->resize = Resize;
    gl->swap = SwapBuffers;
    gl->get_proc_address = GetSymbol;
    gl->destroy = Close;

    return VLC_SUCCESS;

error:
    Close(gl);
    return ret;
}

static int OpenGLES2(vlc_gl_t *gl, unsigned width, unsigned height)
{
    static const struct gl_api api = {
        "OpenGL_ES", EGL_OPENGL_ES_API, 3, EGL_OPENGL_ES2_BIT,
        { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE },
    };
    return Open(gl, &api, width, height);
}

static int OpenGL(vlc_gl_t *gl, unsigned width, unsigned height)
{
    static const struct gl_api api = {
        "OpenGL", EGL_OPENGL_API, 4, EGL_OPENGL_BIT,
        { EGL_NONE },
    };
    return Open(gl, &api, width, height);
}

#ifdef USE_PLATFORM_XCB
# define VLC_PRIORITY 60
#endif
#ifndef VLC_PRIORITY
# define VLC_PRIORITY 50
#endif

vlc_module_begin ()
    set_shortname (N_("EGL"))
    set_description (N_("EGL extension for OpenGL"))
    set_subcategory (SUBCAT_VIDEO_VOUT)
    set_capability("opengl", VLC_PRIORITY)
    set_callback(OpenGL)
    add_shortcut ("egl")

    add_submodule ()
    set_capability("opengl es2", VLC_PRIORITY)
    set_callback(OpenGLES2)
    add_shortcut ("egl")

vlc_module_end ()
