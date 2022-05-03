/**
 * @file input.c
 * @brief Wayland input events for VLC media player
 */
/*****************************************************************************
 * Copyright © 2018 Rémi Denis-Courmont
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

#include <inttypes.h>
#include <stdlib.h>
#include <wayland-client.h>
#include <vlc_common.h>
#include <vlc_vout_window.h>

#include "output.h"

/* TODO: xdg_output protocol */

struct output_list
{
    vout_window_t *owner;
    struct wl_list outputs;
};

struct output_data
{
    vout_window_t *owner;
    struct wl_output *wl_output;
    struct {
        char *name;
        char *description;
    } compositor;

    uint32_t name;
    uint32_t version;
    struct wl_list node;
};

static void output_geometry_cb(void *data, struct wl_output *output,
                               int32_t x, int32_t y, int32_t w, int32_t h,
                               int32_t sp, const char *make, const char *model,
                               int32_t transform)
{
    struct output_data *od = data;
    vout_window_t *wnd = od->owner;

    msg_Dbg(wnd, "output %"PRIu32" geometry: %"PRId32"x%"PRId32"mm"
            "+%"PRId32"+%"PRId32", subpixel %"PRId32", transform %"PRId32,
            od->name, w, h, x, y, sp, transform);

    /* We won't have name/description, so fallback on legacy reporting. */
    if (od->version < 4)
    {
        char idstr[11];
        char *name;
        sprintf(idstr, "%"PRIu32, od->name);
        if (likely(asprintf(&name, "%s - %s", make, model) >= 0))
        {
            vout_window_ReportOutputDevice(wnd, idstr, name);
            free(name);
        }
    }
    (void) output;
}

static void output_mode_cb(void *data, struct wl_output *output,
                           uint32_t flags, int32_t w, int32_t h, int32_t vr)
{
    struct output_data *od = data;
    vout_window_t *wnd = od->owner;
    div_t d = div(vr, 1000);

    msg_Dbg(wnd, "output %"PRIu32" mode: 0x%"PRIx32" %"PRId32"x%"PRId32
            ", %d.%03d Hz", od->name, flags, w, h, d.quot, d.rem);

    (void) output;
}

static void output_done_cb(void *data, struct wl_output *output)
{
    struct output_data *od = data;
    vout_window_t *wnd = od->owner;

    if (od->compositor.name != NULL && od->compositor.description != NULL)
        vout_window_ReportOutputDevice(wnd, od->compositor.name,
                                       od->compositor.description);

    /* Else, we don't have a name/description tuple, fallback on the legacy
     * report with generated name/description from wayland proxy name in the
     * output_geometry_cb. */
    (void) data; (void) output;
}

static void output_scale_cb(void *data, struct wl_output *output, int32_t f)
{
    struct output_data *od = data;
    vout_window_t *wnd = od->owner;

    msg_Dbg(wnd, "output %"PRIu32" scale: %"PRId32, od->name, f);
    (void) output;
}

#ifdef WL_OUTPUT_NAME_SINCE_VERSION
static void output_name_cb(void *data, struct wl_output *output,
                           const char *name)
{
    struct output_data *od = data;
    vout_window_t *wnd = od->owner;

    od->compositor.name = strdup(name);
    msg_Dbg(wnd, "output %"PRIu32" name: %s", od->name, name);
    (void) output;
}
#endif

#ifdef WL_OUTPUT_DESCRIPTION_SINCE_VERSION
static void output_description_cb(void *data, struct wl_output *output,
                                  const char *description)
{
    struct output_data *od = data;
    vout_window_t *wnd = od->owner;

    od->compositor.description = strdup(description);
    msg_Dbg(wnd, "output %"PRIu32" desc: %s", od->name, description);
    (void) output;
}
#endif

static const struct wl_output_listener wl_output_cbs =
{
    output_geometry_cb,
    output_mode_cb,
    output_done_cb,
    output_scale_cb,
#ifdef WL_OUTPUT_NAME_SINCE_VERSION
    .name = output_name_cb,
#endif
#ifdef WL_OUTPUT_DESCRIPTION_SINCE_VERSION
    .description = output_description_cb,
#endif
};

struct output_list *output_list_create(vout_window_t *wnd)
{
    struct output_list *ol = malloc(sizeof (*ol));
    if (unlikely(ol == NULL))
        return NULL;

    ol->owner = wnd;
    wl_list_init(&ol->outputs);
    return ol;
}

int output_create(struct output_list *ol, struct wl_registry *registry,
                  uint32_t name, uint32_t version)
{
    if (unlikely(ol == NULL))
        return -1;

    struct output_data *od = malloc(sizeof (*od));
    if (unlikely(od == NULL))
        return -1;

    if (version > 4)
        version = 4;

#if !defined(WL_OUTPUT_NAME_SINCE_VERSION) || \
    !defined(WL_OUTPUT_DESCRIPTION_SINCE_VERSION)
    if (version > 3)
        version = 3;
#endif

    od->wl_output = wl_registry_bind(registry, name, &wl_output_interface,
                                     version);
    if (unlikely(od->wl_output == NULL))
    {
        free(od);
        return -1;
    }

    od->owner = ol->owner;
    od->name = name;
    od->version = version;
    od->compositor.name = NULL;
    od->compositor.description = NULL;

    wl_output_add_listener(od->wl_output, &wl_output_cbs, od);
    wl_list_insert(&ol->outputs, &od->node);
    return 0;
}

static void output_destroy(struct output_list *ol, struct output_data *od)
{
    if (od->compositor.name != NULL && od->compositor.description != NULL)
    {
        vout_window_ReportOutputDevice(ol->owner, od->compositor.name, NULL);
    }
    else
    {
        char idstr[11];
        sprintf(idstr, "%"PRIu32, od->name);
        vout_window_ReportOutputDevice(ol->owner, idstr, NULL);
    }

    wl_list_remove(&od->node);

    if (od->version >= WL_OUTPUT_RELEASE_SINCE_VERSION)
        wl_output_release(od->wl_output);
    else
        wl_output_destroy(od->wl_output);
    free(od->compositor.name);
    free(od->compositor.description);
    free(od);
}

int output_destroy_by_name(struct output_list *ol, uint32_t name)
{
    if (unlikely(ol == NULL))
        return -1;

    struct wl_list *list = &ol->outputs;
    struct output_data *od;

    wl_list_for_each(od, list, node)
    {
        if (od->name == name)
        {
            output_destroy(ol, od);
            /* Note: return here so no needs for safe walk variant */
            return 0;
        }
    }

    return -1;
}

void output_list_destroy(struct output_list *ol)
{
    if (ol == NULL)
        return;

    struct wl_list *list = &ol->outputs;

    while (!wl_list_empty(list))
        output_destroy(ol, container_of(list->next, struct output_data, node));

    free(ol);
}
