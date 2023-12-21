/*****************************************************************************
 * tracer.c: tracing interface
 * This library provides an interface to the traces to be used by other
 * modules. See vlc_config.h for output configuration.
 *****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
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
# include "config.h"
#endif

#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include <vlc_common.h>
#include <vlc_modules.h>
#include <vlc_tracer.h>
#include "../libvlc.h"

/**
 * Module-based message trace.
 */
struct vlc_tracer_module {
    struct vlc_object_t obj;
    struct vlc_tracer tracer;
    void *opaque;
    const struct vlc_tracer_operations *ops;
};

void vlc_tracer_vaTraceWithTs(struct vlc_tracer *tracer, vlc_tick_t ts,
                              va_list entries)
{
    assert(tracer->ops->trace != NULL);

    va_list copy;
    va_copy(copy, entries);
    tracer->ops->trace(tracer->opaque, ts, copy);
    va_end(copy);
}

void vlc_tracer_TraceWithTs(struct vlc_tracer *tracer, vlc_tick_t ts, ...)
{
    assert(tracer->ops->trace != NULL);

    /* Pass message to the callback */
    va_list entries;
    va_start(entries, ts);
    tracer->ops->trace(tracer->opaque, ts, entries);
    va_end(entries);
}

static int vlc_tracer_load(void *func, bool forced, va_list ap)
{
    vlc_tracer_open_cb activate = func;
    struct vlc_tracer_module *module = va_arg(ap, struct vlc_tracer_module *);

    (void) forced;
    module->ops = activate(VLC_OBJECT(module), &module->opaque);
    return (module->ops != NULL) ? VLC_SUCCESS : VLC_EGENERIC;
}

static void vlc_tracer_module_Trace(void *opaque, vlc_tick_t ts,
                                    va_list entries)
{
    struct vlc_tracer_module *module = opaque;
    module->ops->trace(module->opaque, ts, entries);
}

static void vlc_tracer_module_Destroy(void *opaque)
{
    struct vlc_tracer_module *module = opaque;

    if (module->ops->destroy != NULL)
        module->ops->destroy(module->opaque);

    vlc_object_delete(VLC_OBJECT(module));
}

struct vlc_tracer *vlc_tracer_Create(vlc_object_t *parent, const char *module_name)
{
    struct vlc_tracer_module *module;

    module = vlc_custom_create(parent, sizeof (*module), "tracer");
    if (unlikely(module == NULL))
        return NULL;

    if (vlc_module_load(vlc_object_logger(module), "tracer", module_name, false,
                        vlc_tracer_load, module) == NULL) {
        vlc_object_delete(VLC_OBJECT(module));
        return NULL;
    }

    static const struct vlc_tracer_operations vlc_tracer_module_ops =
    {
        .trace = vlc_tracer_module_Trace,
        .destroy = vlc_tracer_module_Destroy,
    };

    module->tracer.opaque = module;
    module->tracer.ops = &vlc_tracer_module_ops;

    return &module->tracer;
}

void vlc_tracer_Destroy(struct vlc_tracer *tracer)
{
    if (tracer->ops->destroy != NULL)
        tracer->ops->destroy(tracer->opaque);
}
