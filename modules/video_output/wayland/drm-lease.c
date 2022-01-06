/**
 * @file drm-lease.c
 * @brief DRM lease surface provider module for VLC media player
 */
/*****************************************************************************
 * Copyright © 2021 Videolabs
 *
 * Authors: Alexandre Janniaux <ajanni@videolabs.io>
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
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>

#include <wayland-client.h>

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_vout_window.h>
#include <vlc_fs.h>

#include "drm-lease-client-protocol.h"

#include <xf86drm.h>
#include <xf86drmMode.h>

typedef enum { drvSuccess, drvTryNext, drvFail } deviceRval;

typedef struct
{
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct wp_drm_lease_device_v1 *lease_device;
    struct wp_drm_lease_connector_v1 *lease_connector;
    struct wp_drm_lease_request_v1 *request;
    struct wp_drm_lease_v1 *lease;

    int drm_fd;
    int drm_lease_fd;
    int crtc;
    unsigned width;
    unsigned height;

    drmModeModeInfo *mode;
    drmModeConnector *conn;
    uint32_t connector;
    drmModeCrtc *saved_crtc;

    uint32_t framebuffer;
    uint32_t main_buffer;

    vlc_mutex_t lock;
    vlc_thread_t thread;
} vout_window_sys_t;

static bool IsLeased(int drm_fd, uint32_t object)
{
    drmModeObjectListPtr lease = drmModeGetLease(drm_fd);
    for (uint32_t j = 0; j < lease->count; ++j) {
        if (object == lease->objects[j])
            return true;
    }
    return false;
}

static deviceRval FindCRTC(vout_window_t *wnd, drmModeRes const *res,
                             drmModeConnector const *conn)
{
    vout_window_sys_t *sys = wnd->sys;
    drmModeEncoder *enc;
    int i, j;

    /*
     * Try current encoder and CRTC combo
     */
    if (conn->encoder_id) {
        enc = drmModeGetEncoder(sys->drm_fd, conn->encoder_id);
        if (enc) {
            if (enc->crtc_id) {
                msg_Dbg(wnd, "Got CRTC %d from current encoder", enc->crtc_id);

                sys->crtc = enc->crtc_id;
                drmModeFreeEncoder(enc);
                return drvSuccess;
            }
            drmModeFreeEncoder(enc);
        }
    }

    /*
     * Iterate all available encoders to find CRTC
     */
    for (i = 0; i < conn->count_encoders; i++) {
        msg_Info(wnd, " -- Compatible encoder %d", i);
        enc = drmModeGetEncoder(sys->drm_fd, conn->encoders[i]);

        for (j = 0; enc && j < res->count_crtcs; ++j) {
            if (!IsLeased(sys->drm_lease_fd, res->crtcs[j]))
                continue;

            if ((enc->possible_crtcs & (1 << j)) != 0 && res->crtcs[j]) {
                sys->crtc = res->crtcs[j];
                msg_Info(wnd, " -- Compatible CRTC %d", sys->crtc);
                drmModeFreeEncoder(enc);
                return drvSuccess;
            }
        }
        drmModeFreeEncoder(enc);
    }

    msg_Err(wnd , "Cannot find CRTC for connector %d", conn->connector_id);
    return drvTryNext;
}

static deviceRval SetupDevice(vout_window_t *wnd, drmModeRes const *res,
                             drmModeConnector const *conn)
{
    vout_window_sys_t *sys = wnd->sys;
    deviceRval ret;

    if (conn->connection != DRM_MODE_CONNECTED || conn->count_modes == 0)
        return drvTryNext;

    sys->width = conn->modes[0].hdisplay;
    sys->height = conn->modes[0].vdisplay;
    sys->mode = &conn->modes[0];
    msg_Dbg(wnd, "Mode resolution for connector %u is %ux%u",
            conn->connector_id, sys->width, sys->height);

    ret = FindCRTC(wnd, res, conn);
    if (ret != drvSuccess) {
        msg_Dbg(wnd, "No valid CRTC for connector %d", conn->connector_id);
        return ret;
    }
    return drvSuccess;
}

static int Enable(vout_window_t *wnd, const vout_window_cfg_t *restrict cfg)
{
    vout_window_sys_t *sys = wnd->sys;
    //struct wl_display *display = wnd->display.wl;
    (void)wnd; (void)cfg;

    bool found_connector = false;

    int nconn = 0;
    drmModeResPtr res = drmModeGetResources(sys->drm_lease_fd);
    for (int i = 0; i < res->count_connectors; ++i) {
        if (!IsLeased(sys->drm_lease_fd, res->connectors[i]))
            continue;
        drmModeConnector *conn =
            drmModeGetConnector(sys->drm_lease_fd, res->connectors[i]);
        if (conn == NULL)
            continue;

        found_connector = true;

        int ret = SetupDevice(wnd, res, conn);
        if (ret != drvSuccess) {
            if (ret != drvTryNext) {
                msg_Err(wnd, "Cannot do setup for connector %u:%u", i,
                        res->connectors[i]);

                drmModeFreeConnector(conn);
                drmModeFreeResources(res);
                return VLC_EGENERIC;
            }
            drmModeFreeConnector(conn);
            found_connector = false;
            msg_Dbg(wnd, " - Connector %d: could not setup device", i);
            continue;
        }
        sys->connector = conn->connector_id;
        sys->conn = conn;

        ++nconn;
        break;
    }
    drmModeFreeResources(res);

    if (!found_connector)
    {
        msg_Warn(wnd, "Could not find a valid connector");
        return VLC_EGENERIC;
    }

    wnd->handle.crtc = sys->crtc;
    /* Store current KMS state before modifying */
    sys->saved_crtc = drmModeGetCrtc(sys->drm_lease_fd, sys->crtc);

    /*
     * Create a new framebuffer to avoid compositing the planes into the saved
     * framebuffer for the current CRTC.
     */

    struct drm_mode_create_dumb request = {
        .width = sys->width, .height = sys->height, .bpp = 32
    };

    int ret = drmIoctl(sys->drm_lease_fd, DRM_IOCTL_MODE_CREATE_DUMB, &request);
    if (ret != drvSuccess)
    {
        msg_Err(wnd, "Cannot create the dumb buffer");
        goto error_create_dumb;
    }

    uint32_t new_fb;
    ret = drmModeAddFB(sys->drm_lease_fd, sys->width, sys->height, 24, 32, request.pitch,
            request.handle, &new_fb);
    if (ret != drvSuccess)
    {
        msg_Err(wnd, "Cannot create the framebuffer");
        goto error_add_fb;
    }

    ret = drmModeSetCrtc(sys->drm_lease_fd, sys->crtc, new_fb, 0, 0, &sys->connector, 1, sys->mode);
    if (ret != drvSuccess)
    {
        msg_Err(wnd, "Cannot set the CRTC");
        goto error_set_crtc;
    }

    sys->framebuffer = new_fb;
    sys->main_buffer = request.handle;

    vout_window_ReportSize(wnd, sys->width, sys->height);

    return VLC_SUCCESS;

error_set_crtc:
    drmModeRmFB(sys->drm_lease_fd, new_fb);

error_add_fb:
    struct drm_mode_destroy_dumb destroy_request = {
        .handle = request.handle
    };
    ret = drmIoctl(sys->drm_fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destroy_request);
    /* This must be a programmation error if we cannot destroy the resources
     * we created. */
    assert(ret == drvSuccess);

error_create_dumb:
    drmModeFreeCrtc(sys->saved_crtc);
    drmModeFreeConnector(sys->conn);

    return VLC_EGENERIC;
}

static void register_drm_lease_device(void *opaque,
        struct wl_registry *registry, uint32_t name, uint32_t version)
{
    vout_window_t *wnd = opaque;
    vout_window_sys_t *sys = wnd->sys;;
    sys->lease_device = wl_registry_bind(registry, name,
            &wp_drm_lease_device_v1_interface, 1);
    (void)version;
}

struct registry_handler
{
    const char *iface;
    void (*global)(void *, struct wl_registry *, uint32_t, uint32_t);
    uint32_t max_version;
};

static const struct registry_handler global_handlers[] =
{
     // { "wl_compositor", register_wl_compositor, 2 },
     // { "wl_output", register_wl_output, 1},
     // { "wl_seat", register_wl_seat, UINT32_C(-1) },
     // { "xdg_wm_base", register_xdg_wm_base, 1 },
     // { "zxdg_decoration_manager_v1", register_xdg_decoration_manager, 1 },
    { "wp_drm_lease_device_v1", register_drm_lease_device, 1 },
};

static int rghcmp(const void *a, const void *b)
{
    const char *iface = a;
    const struct registry_handler *handler = b;

    return strcmp(iface, handler->iface);
}

static void registry_global_cb(void *data, struct wl_registry *registry,
                               uint32_t name, const char *iface, uint32_t vers)
{
    vout_window_t *wnd = data;
    const struct registry_handler *h;

    msg_Dbg(wnd, "global %3"PRIu32": %s version %"PRIu32, name, iface, vers);

    h = bsearch(iface, global_handlers, ARRAY_SIZE(global_handlers),
                sizeof (*h), rghcmp);
    if (h != NULL)
    {
        uint32_t version = (vers < h->max_version) ? vers : h->max_version;

        h->global(data, registry, name, version);
    }
}

static void registry_global_remove_cb(void *data, struct wl_registry *registry,
                                      uint32_t name)
{
    vout_window_t *wnd = data;
    //vout_window_sys_t *sys = wnd->sys;

    msg_Dbg(wnd, "global remove %3"PRIu32, name);
    (void) registry;
}

static const struct wl_registry_listener registry_cbs =
{
    registry_global_cb,
    registry_global_remove_cb,
};

static void
lease_fd(void *data, struct wp_drm_lease_v1 *lease, int32_t leased_fd)
{
    vout_window_t *wnd = data;
    vout_window_sys_t *sys = wnd->sys;

    msg_Dbg(wnd, "lease fd %d", leased_fd);
    sys->drm_lease_fd = leased_fd;
    sys->lease = lease;
    (void)lease;
}

static void
lease_finished(void *data, struct wp_drm_lease_v1 *lease)
{
    vout_window_t *wnd = data;

    msg_Dbg(wnd, "finishing lease fd");
    //close(*fd);
    (void)lease;
}

static const struct wp_drm_lease_v1_listener lease_listener = {
    .lease_fd = lease_fd,
    .finished = lease_finished,
};

static void
lease_connector_name(void *data, struct wp_drm_lease_connector_v1 *conn,
        const char *name)
{
    vout_window_t *wnd = data;
    msg_Dbg(wnd, "lease connector name %s\n", name);
    (void)conn;
}

static void
lease_connector_desc(void *data, struct wp_drm_lease_connector_v1 *conn,
        const char *desc)
{
    vout_window_t *wnd = data;
    msg_Dbg(wnd, "lease connector description %s\n", desc);
    (void)conn;
}

static void
lease_connector_id(void *data, struct wp_drm_lease_connector_v1 *conn,
        uint32_t id)
{
    vout_window_t *wnd = data;
    msg_Dbg(wnd, "lease connector id %zu", (size_t)id);
    (void)conn;
}

static void
lease_connector_withdrawn(void *data, struct wp_drm_lease_connector_v1 *conn)
{
    vout_window_t *wnd = data;
    msg_Dbg(wnd, "lease connector withdrawn");
    (void)conn;
}

static void
lease_connector_done(void *data, struct wp_drm_lease_connector_v1 *conn)
{
    vout_window_t *wnd = data;
    msg_Dbg(wnd, "lease connector done");
    (void)conn;
}

static const struct wp_drm_lease_connector_v1_listener lease_connector_listener = {
    .name = lease_connector_name,
    .description = lease_connector_desc,
    .connector_id = lease_connector_id,
    .done = lease_connector_done,
    .withdrawn = lease_connector_withdrawn,
};

static void
lease_device_drm_fd(void *data, struct wp_drm_lease_device_v1 *dev, int32_t fd)
{
    vout_window_t *wnd = data;
    vout_window_sys_t *sys = wnd->sys;

    msg_Info(wnd, "lease device drm_fd %d", fd);
    sys->drm_fd = fd;
    (void)dev;
}

static void
lease_device_connector(void *data, struct wp_drm_lease_device_v1 *dev,
        struct wp_drm_lease_connector_v1 *conn)
{
    vout_window_t *wnd = data;
    vout_window_sys_t *sys = wnd->sys;

    msg_Info(wnd, "lease device connector");
    if (!sys->lease_connector) {
        sys->lease_connector = conn;
        wp_drm_lease_connector_v1_add_listener(sys->lease_connector,
                &lease_connector_listener, wnd);
    } else {
        msg_Warn(wnd, "state already has a connector\n");
    }
    (void)dev;
}

static void
lease_device_done(void *data, struct wp_drm_lease_device_v1 *dev)
{
    vout_window_t *wnd = data;

    msg_Info(wnd, "lease device connector done");
    (void)dev;
}

static void
lease_device_release(void *data, struct wp_drm_lease_device_v1 *dev)
{
    vout_window_t *wnd = data;

    msg_Info(wnd, "released lease device connector");
    (void)dev;

}

static const struct wp_drm_lease_device_v1_listener lease_device_listener = {
    .drm_fd = lease_device_drm_fd,
    .connector = lease_device_connector,
    .done = lease_device_done,
    .released = lease_device_release,
};


//static const struct wp_drm_lease_request_v1_listener lease_request_listener = {
//
//};

static void Close(vout_window_t *wnd)
{
    vout_window_sys_t *sys = wnd->sys;
    wp_drm_lease_device_v1_release(sys->lease_device);

    wp_drm_lease_v1_destroy(sys->lease);
    wp_drm_lease_connector_v1_destroy(sys->lease_connector);
    // TODO: wait released

    wp_drm_lease_device_v1_destroy(sys->lease_device);

    wl_registry_destroy(sys->registry);
    wl_display_disconnect(sys->display);
    free(sys);
}

static int Open(vout_window_t *wnd)
{
    vout_window_sys_t *sys = malloc(sizeof (*sys));
    if (unlikely(sys == NULL))
        return VLC_ENOMEM;

    vlc_mutex_init(&sys->lock);
    wnd->sys = sys;
    wnd->handle.wl = NULL;

    sys->registry = NULL;
    sys->lease_connector = NULL;
    sys->compositor = NULL;
    sys->lease_device = NULL;
    sys->request = NULL;
    sys->lease = NULL;
    sys->drm_lease_fd = -1;

    /* Connect to the display server */
    char *dpy_name = var_InheritString(wnd, "wl-display");
    struct wl_display *display = wl_display_connect(dpy_name);
    free(dpy_name);

    if (display == NULL)
    {
        msg_Dbg(wnd, "No wayland display available.");
        free(sys);
        return VLC_EGENERIC;
    }

    /* Find the interesting singleton(s) */
    sys->registry = wl_display_get_registry(display);
    if (sys->registry == NULL)
        goto error;

    wl_registry_add_listener(sys->registry, &registry_cbs, wnd);
    wl_display_roundtrip(display); /* complete registry enumeration */

    if (sys->lease_device == NULL)
    {
        msg_Dbg(wnd, "no wayland drm lease device available.");
        goto error;
    }

    wp_drm_lease_device_v1_add_listener(sys->lease_device,
            &lease_device_listener, wnd);
    wl_display_roundtrip(display); /* complete lease enumeration */
    msg_Dbg(wnd, "wayland drm lease device available.");

    if (sys->drm_fd == -1)
        goto error;

char *primary_device = drmGetPrimaryDeviceNameFromFd(sys->drm_fd);
    if (primary_device)
        msg_Dbg(wnd, "Primary device: %s", primary_device);
    free(primary_device);

    char *render_device = drmGetRenderDeviceNameFromFd(sys->drm_fd);
    if (render_device)
        msg_Dbg(wnd, "Render device: %s", render_device);
    free(render_device);

    if (sys->lease_connector == NULL)
    {
        msg_Dbg(wnd, "No connector has been leased");
        goto error;
    }

    sys->request =
        wp_drm_lease_device_v1_create_lease_request(sys->lease_device);
    if (sys->request == NULL)
        goto error;

    wp_drm_lease_request_v1_request_connector(sys->request, sys->lease_connector);
    struct wp_drm_lease_v1 *lease =
        wp_drm_lease_request_v1_submit(sys->request);
    wp_drm_lease_v1_add_listener(lease, &lease_listener, wnd);
    wl_display_roundtrip(display);

    if (sys->drm_lease_fd == -1)
        goto error;

    drmModeObjectListPtr lease_list = drmModeGetLease(sys->drm_lease_fd);
    msg_Dbg(wnd, "Got the leased objects:");
    for (uint32_t j = 0; j < lease_list->count; ++j) {
        msg_Info(wnd, " - %d", lease_list->objects[j]);
    }

    static const struct vout_window_operations ops = {
        .enable = Enable,
        .destroy = Close,
    };
    wnd->type = VOUT_WINDOW_TYPE_KMS;
    wnd->display.drm_fd = sys->drm_lease_fd;
    wnd->ops = &ops;
    sys->display = display;

    return VLC_SUCCESS;

error:
    if (sys->lease_device)
        wp_drm_lease_device_v1_release(sys->lease_device);
    if (sys->lease != NULL)
        wp_drm_lease_v1_destroy(sys->lease);
    if (sys->lease_device != NULL)
        wp_drm_lease_device_v1_destroy(sys->lease_device);
    if (sys->registry != NULL)
        wl_registry_destroy(sys->registry);
    wl_display_disconnect(display);

    if (sys->drm_fd != -1)
        vlc_close(sys->drm_fd);
    if (sys->drm_lease_fd != -1)
        vlc_close(sys->drm_lease_fd);

    free(sys);
    return VLC_EGENERIC;
}

#define DISPLAY_TEXT N_("Wayland display")
#define DISPLAY_LONGTEXT N_( \
    "Video will be rendered with this Wayland display. " \
    "If empty, the default display will be used.")

#define OUTPUT_TEXT N_("Fullscreen output")
#define OUTPUT_LONGTEXT N_( \
    "Fullscreen mode with use the output with this name by default.")

vlc_module_begin()
    set_shortname(N_("Wayland DRM lease"))
    set_description(N_("Wayland DRM lease surface"))
    set_category(CAT_VIDEO)
    set_subcategory(SUBCAT_VIDEO_VOUT)
    set_capability("vout window", 0)
    set_callback(Open)

    add_string("drm-lease-display", NULL, DISPLAY_TEXT, DISPLAY_LONGTEXT)
    add_integer("drm-lease-output", 0, OUTPUT_TEXT, OUTPUT_LONGTEXT)
        change_integer_range(0, UINT32_MAX)
        change_volatile()
vlc_module_end()
