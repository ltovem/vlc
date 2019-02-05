/**
 * @file shm.c
 * @brief Wayland shared memory video output module for VLC media player
 */
/*****************************************************************************
 * Copyright © 2014, 2017 Rémi Denis-Courmont
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
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include <wayland-client.h>
#include "viewporter-client-protocol.h"
#include "registry.h"

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_vout_display.h>
#include <vlc_fs.h>

#define MAX_PICTURES 4

typedef struct vout_display_sys_t
{
    vlc_window_t *embed; /* VLC window */
    struct wl_event_queue *eventq;
    struct wl_shm *shm;
    struct wp_viewporter *viewporter;
    struct wp_viewport *viewport;
    struct wl_compositor *compositor;
    struct wl_subcompositor *subcompositor;

    struct wl_surface *surface;
    struct wl_subsurface *subsurface;

    struct
    {
        struct wl_buffer *buffer;
        struct wp_viewport *viewport;
    } back_bg;

    size_t active_buffers;
} vout_display_sys_t;

struct buffer_data
{
    picture_t *picture;
    size_t *counter;
};

static_assert (sizeof (vout_display_sys_t) >= MAX_PICTURES,
               "Pointer arithmetic error");

static void buffer_release_cb(void *data, struct wl_buffer *buffer)
{
    struct buffer_data *d = data;

    picture_Release(d->picture);
    (*(d->counter))--;
    free(d);
    wl_buffer_destroy(buffer);
}

static const struct wl_buffer_listener buffer_cbs =
{
    buffer_release_cb,
};

static void background_buffer_release_cb(void *data, struct wl_buffer *buffer)
{
    vout_display_t *vd = data;
    vout_display_sys_t *sys = vd->sys;

    if (sys->back_bg.buffer == buffer)
        sys->back_bg.buffer = NULL;

    wl_buffer_destroy(buffer);
}

static const struct wl_buffer_listener background_buffer_cbs =
{
    background_buffer_release_cb,
};

static struct wl_buffer* CreateBackgroundBuffer(struct wl_shm *shm,
                                                uint32_t width,
                                                uint32_t height)
{
    int back_bg_fd = vlc_open("/dev/zero", O_RDWR);
    if (back_bg_fd < 0)
        return NULL;

    struct wl_shm_pool *pool = wl_shm_create_pool(shm, back_bg_fd,
                                                  width*height);
    vlc_close(back_bg_fd);

    if (pool == NULL)
        return NULL;

    struct wl_buffer *buffer =
        wl_shm_pool_create_buffer(pool, 0, width, height, width,
                                  WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);

    return buffer;
}

static void Prepare(vout_display_t *vd, picture_t *pic, subpicture_t *subpic,
                    vlc_tick_t date)
{
    VLC_UNUSED(date);
    vout_display_sys_t *sys = vd->sys;
    struct wl_display *display = sys->embed->display.wl;
    struct wl_surface *surface = sys->surface;
    struct picture_buffer_t *picbuf = pic->p_sys;

    if (picbuf->fd == -1)
        return;

    struct buffer_data *d = malloc(sizeof (*d));
    if (unlikely(d == NULL))
        return;

    d->picture = pic;
    d->counter = &sys->active_buffers;

    off_t offset = picbuf->offset;
    const size_t stride = pic->p->i_pitch;
    const size_t size = pic->p->i_lines * stride;
    struct wl_shm_pool *pool;
    struct wl_buffer *buf;

    pool = wl_shm_create_pool(sys->shm, picbuf->fd, offset + size);
    if (pool == NULL)
    {
        free(d);
        return;
    }

    if (sys->viewport == NULL) /* Poor man's crop */
        offset += 4 * vd->fmt->i_x_offset
                  + pic->p->i_pitch * vd->fmt->i_y_offset;

    buf = wl_shm_pool_create_buffer(pool, offset, vd->fmt->i_visible_width,
                                    vd->fmt->i_visible_height, stride,
                                    WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    if (buf == NULL)
    {
        free(d);
        return;
    }

    picture_Hold(pic);

    wl_buffer_add_listener(buf, &buffer_cbs, d);
    wl_surface_attach(surface, buf, 0, 0);
    wl_surface_damage(surface, 0, 0, vd->cfg->display.width, vd->cfg->display.height);
    wl_display_flush(display);

    sys->active_buffers++;

    (void) subpic;
}

static void Display(vout_display_t *vd, picture_t *pic)
{
    vout_display_sys_t *sys = vd->sys;
    struct wl_display *display = sys->embed->display.wl;

    wl_surface_commit(sys->surface);
    wl_display_roundtrip_queue(display, sys->eventq);

    (void) pic;
}

static int ResetPictures(vout_display_t *vd, video_format_t *fmt)
{
    vout_display_place_t place;
    video_format_t src;
    vout_display_sys_t *sys = vd->sys;
    assert(sys->viewport == NULL);

    vout_display_PlacePicture(&place, vd->source, &vd->cfg->display);
    video_format_ApplyRotation(&src, vd->source);

    fmt->i_width  = src.i_width * place.width
                                / src.i_visible_width;
    fmt->i_height = src.i_height * place.height
                                    / src.i_visible_height;
    fmt->i_visible_width  = place.width;
    fmt->i_visible_height = place.height;
    fmt->i_x_offset = src.i_x_offset * place.width
                                        / src.i_visible_width;
    fmt->i_y_offset = src.i_y_offset * place.height
                                        / src.i_visible_height;
    return VLC_SUCCESS;
}

static int Control(vout_display_t *vd, int query)
{
    vout_display_sys_t *sys = vd->sys;

    switch (query)
    {
        case VOUT_DISPLAY_CHANGE_DISPLAY_SIZE:
        case VOUT_DISPLAY_CHANGE_DISPLAY_FILLED:
        case VOUT_DISPLAY_CHANGE_ZOOM:
        case VOUT_DISPLAY_CHANGE_SOURCE_ASPECT:
        case VOUT_DISPLAY_CHANGE_SOURCE_CROP:
        {
            video_format_t fmt;
            vout_display_place_t place;

            video_format_ApplyRotation(&fmt, vd->fmt);
            vout_display_PlacePicture(&place, vd->fmt, &vd->cfg->display);

            if (sys->viewport != NULL)
            {
                wp_viewport_set_source(sys->viewport,
                                wl_fixed_from_int(fmt.i_x_offset),
                                wl_fixed_from_int(fmt.i_y_offset),
                                wl_fixed_from_int(fmt.i_visible_width),
                                wl_fixed_from_int(fmt.i_visible_height));
                wp_viewport_set_destination(sys->viewport,
                                            place.width, place.height);

            }

            wl_subsurface_set_position(sys->subsurface, place.x, place.y);

            if (sys->back_bg.viewport)
            {
                wp_viewport_set_source(sys->back_bg.viewport,
                                       wl_fixed_from_int(0),
                                       wl_fixed_from_int(0),
                                       wl_fixed_from_int(1),
                                       wl_fixed_from_int(1));
                wp_viewport_set_destination(sys->back_bg.viewport,
                                            vd->cfg->display.width,
                                            vd->cfg->display.height);
            }
            else
            {
                sys->back_bg.buffer =
                    CreateBackgroundBuffer(sys->shm,
                                           vd->cfg->display.width,
                                           vd->cfg->display.height);
                if (sys->back_bg.buffer)
                {
                    wl_buffer_add_listener(sys->back_bg.buffer,
                                           &background_buffer_cbs, vd);
                    wl_surface_attach(sys->embed->handle.wl,
                                      sys->back_bg.buffer, 0, 0);
                    wl_surface_damage(sys->embed->handle.wl, 0, 0,
                                      vd->cfg->display.width,
                                      vd->cfg->display.height);
                }
            }

            /* We have to commit the parent surface to commit
             *  + the subsurface position
             *  + the attached black buffer or the background viewport */
            wl_surface_commit(sys->embed->handle.wl);

            /* Signal vlc core that we don't know how to resize. It will
             * trigger another method of resizing. */
            if (sys->viewport == NULL)
                return VLC_EGENERIC;

            break;
        }
        default:
             msg_Err(vd, "unknown request %d", query);
             return VLC_EGENERIC;
    }
    return VLC_SUCCESS;
}

static void shm_format_cb(void *data, struct wl_shm *shm, uint32_t format)
{
    vout_display_t *vd = data;
    char str[4];

    memcpy(str, &format, sizeof (str));

    if (format >= 0x20202020)
        msg_Dbg(vd, "format %.4s (0x%08"PRIx32")", str, format);
    else
        msg_Dbg(vd, "format %4"PRIu32" (0x%08"PRIx32")", format, format);
    (void) shm;
}

static const struct wl_shm_listener shm_cbs =
{
    shm_format_cb,
};

static void Close(vout_display_t *vd)
{
    vout_display_sys_t *sys = vd->sys;
    struct wl_display *display = sys->embed->display.wl;
    struct wl_surface *surface = sys->embed->handle.wl;

    wl_surface_attach(surface, NULL, 0, 0);
    wl_surface_commit(surface);

    wl_surface_attach(sys->surface, NULL, 0, 0);
    wl_surface_commit(sys->surface);

    /* Wait until all picture buffers are released by the server */
    while (sys->active_buffers > 0) {
        msg_Dbg(vd, "%zu buffer(s) still active", sys->active_buffers);
        wl_display_roundtrip_queue(display, sys->eventq);
    }
    msg_Dbg(vd, "no active buffers left");

    if (sys->viewport != NULL)
        wp_viewport_destroy(sys->viewport);

    if (sys->back_bg.viewport)
        wp_viewport_destroy(sys->back_bg.viewport);

    if (sys->back_bg.buffer)
        wl_buffer_destroy(sys->back_bg.buffer);

    if (sys->viewporter != NULL)
        wp_viewporter_destroy(sys->viewporter);

    wl_subsurface_destroy(sys->subsurface);
    wl_surface_destroy(sys->surface);
    wl_subcompositor_destroy(sys->subcompositor);
    wl_compositor_destroy(sys->compositor);
    wl_shm_destroy(sys->shm);
    wl_display_flush(display);
    wl_event_queue_destroy(sys->eventq);
    free(sys);
}

static const struct vlc_display_operations ops = {
    .close = Close,
    .prepare = Prepare,
    .display = Display,
    .control = Control,
    .reset_pictures = ResetPictures,
};

static int Open(vout_display_t *vd,
                video_format_t *fmtp, vlc_video_context *context)
{
    if (vd->cfg->window->type != VLC_WINDOW_TYPE_WAYLAND)
        return VLC_EGENERIC;

    vout_display_sys_t *sys = malloc(sizeof (*sys));
    if (unlikely(sys == NULL))
        return VLC_ENOMEM;

    vd->sys = sys;
    sys->embed = NULL;
    sys->eventq = NULL;
    sys->shm = NULL;
    sys->active_buffers = 0;

    // TODO
    sys->viewporter = NULL;
    sys->compositor = NULL;
    sys->subcompositor = NULL;
    //sys->use_buffer_transform = false;
    sys->surface = NULL;
    sys->subsurface = NULL;

    sys->back_bg.buffer = NULL;
    sys->back_bg.viewport = NULL;

    /* Get window */
    sys->embed = vd->cfg->window;
    assert(sys->embed != NULL);

    struct wl_display *display = sys->embed->display.wl;
    struct vlc_wl_registry *registry = NULL;

    sys->eventq = wl_display_create_queue(display);
    if (sys->eventq == NULL)
        goto error;

    registry = vlc_wl_registry_get(display, sys->eventq);
    if (registry == NULL)
        goto error;

    sys->shm = vlc_wl_shm_get(registry);
    if (sys->shm == NULL)
        goto error;

    sys->viewporter = (struct wp_viewporter *)
                      vlc_wl_interface_bind(registry, "wp_viewporter",
                                            &wp_viewporter_interface, NULL);

    sys->compositor = (struct wl_compositor *)
                      vlc_wl_interface_bind(registry, "wl_compositor",
                                            &wl_compositor_interface, NULL);

    sys->subcompositor = (struct wl_subcompositor *)
                         vlc_wl_interface_bind(registry, "wl_subcompositor",
                                               &wl_subcompositor_interface, NULL);

    if (sys->compositor == NULL || sys->subcompositor == NULL)
        goto error;

    wl_shm_add_listener(sys->shm, &shm_cbs, vd);
    wl_display_roundtrip_queue(display, sys->eventq);

    /* Create the subsurface containing the video. */
    sys->surface = wl_compositor_create_surface(sys->compositor);
    if (!sys->surface)
        goto error;

    sys->subsurface = wl_subcompositor_get_subsurface(sys->subcompositor,
                                                      sys->surface,
                                                      sys->embed->handle.wl);
    if (!sys->subsurface)
        goto error;

    /* UI surface is updated at a different frequency than the video so we
     * need to be in desync mode.
     * TODO: When resizing, we need to set sync mode and commit the whole chain
     * of resize, and then set the surface back to desync mode.*/
    wl_subsurface_set_desync(sys->subsurface);
    wl_subsurface_place_above(sys->subsurface, sys->embed->handle.wl);

    /* We own the subsurface and we don't want to handle mouse and keyboard
     * events here as this is a vout display module, so don't capture the events
     * and let the vout window module handle them through its own surface, which
     * is guaranteed to be at least as large as the video surface. */
    struct wl_region *input_region =
        wl_compositor_create_region(sys->compositor);
    wl_region_add(input_region, 0, 0, 0, 0);
    wl_surface_set_input_region(sys->surface, input_region);
    wl_region_destroy(input_region);

    if (sys->viewporter != NULL)
        sys->viewport = wp_viewporter_get_viewport(sys->viewporter,
                                                   sys->surface);
    else
        sys->viewport = NULL;

    /* Determine our pixel format */
    static const enum wl_output_transform transforms[8] = {
        [ORIENT_TOP_LEFT] = WL_OUTPUT_TRANSFORM_NORMAL,
        [ORIENT_TOP_RIGHT] = WL_OUTPUT_TRANSFORM_FLIPPED,
        [ORIENT_BOTTOM_LEFT] = WL_OUTPUT_TRANSFORM_FLIPPED_180,
        [ORIENT_BOTTOM_RIGHT] = WL_OUTPUT_TRANSFORM_180,
        [ORIENT_LEFT_TOP] = WL_OUTPUT_TRANSFORM_FLIPPED_270,
        [ORIENT_LEFT_BOTTOM] = WL_OUTPUT_TRANSFORM_90,
        [ORIENT_RIGHT_TOP] = WL_OUTPUT_TRANSFORM_270,
        [ORIENT_RIGHT_BOTTOM] = WL_OUTPUT_TRANSFORM_FLIPPED_90,
    };

    if (vlc_wl_interface_get_version(registry, "wl_subcompositor") >= 2)
    {
        wl_surface_set_buffer_transform(sys->surface,
                                        transforms[fmtp->orientation]);
    }
    else
    {
        video_format_t fmt = *fmtp;
        video_format_ApplyRotation(fmtp, &fmt);
    }

    /* When the viewporter is available, it's more efficient to rescale a
     * 1x1 pixel buffer than to re-upload large black textures each time the
     * window size is changed. In any case, the mecanism ensure the parent
     * surface size is at least the size of the video surface. */
    if (sys->viewporter != NULL)
    {
        sys->back_bg.viewport = wp_viewporter_get_viewport(sys->viewporter, sys->embed->handle.wl);
        sys->back_bg.buffer = CreateBackgroundBuffer(sys->shm, 1, 1);
    }
    else
    {
        sys->back_bg.viewport = NULL;
        sys->back_bg.buffer = CreateBackgroundBuffer(sys->shm,
                                                     vd->cfg->display.width,
                                                     vd->cfg->display.height);
    }

    if (sys->back_bg.buffer == NULL)
        goto error;
    else
        wl_buffer_add_listener(sys->back_bg.buffer, &background_buffer_cbs, vd);

    wl_surface_attach(sys->embed->handle.wl, sys->back_bg.buffer, 0, 0);
    wl_surface_damage(sys->embed->handle.wl, 0, 0,
                      vd->cfg->display.width, vd->cfg->display.height);
    wl_surface_commit(sys->embed->handle.wl);

    fmtp->i_chroma = VLC_CODEC_RGB32;

    vd->ops = &ops;

    vlc_wl_registry_destroy(registry);
    (void) context;
    return VLC_SUCCESS;

error:
    if (sys->subsurface != NULL)
        wl_subsurface_destroy(sys->subsurface);

    if (sys->surface != NULL)
        wl_surface_destroy(sys->surface);

    if (sys->viewporter != NULL)
        wp_viewporter_destroy(sys->viewporter);

    if (sys->subcompositor != NULL)
        wl_subcompositor_destroy(sys->subcompositor);

    if (sys->compositor != NULL)
        wl_compositor_destroy(sys->compositor);

    if (sys->shm != NULL)
        wl_shm_destroy(sys->shm);

    if (registry != NULL)
        vlc_wl_registry_destroy(registry);

    if (sys->eventq != NULL)
        wl_event_queue_destroy(sys->eventq);
    free(sys);
    return VLC_EGENERIC;
}

vlc_module_begin()
    set_shortname(N_("WL SHM"))
    set_description(N_("Wayland shared memory video output"))
    set_subcategory(SUBCAT_VIDEO_VOUT)
    set_callback_display(Open, 170)
    add_shortcut("wl")
vlc_module_end()
