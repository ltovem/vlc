/*****************************************************************************
 * libvlc.c: libvlc instances creation and deletion, interfaces handling
 *****************************************************************************
 * Copyright (C) 1998-2008 VLC authors and VideoLAN
 * Copyright (C) 2023      Videolabs
 *
 * Authors: Vincent Seguin <seguin@via.ecp.fr>
 *          Samuel Hocevar <sam@zoy.org>
 *          Gildas Bazin <gbazin@videolan.org>
 *          Derk-Jan Hartman <hartman at videolan dot org>
 *          Rémi Denis-Courmont
 *          Alexandre Janniaux <ajanni@videolabs.io>
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

#include <vlc_common.h>
#include <vlc_tracer.h>
#include <vlc_interface.h>

#include "../lib/libvlc_internal.h"
#include "libvlc.h"

#include "misc/rcu.h"

/* No-op tracer */
static void vlc_tracer_discard_Trace(void *opaque, vlc_tick_t ts,
                                     va_list entries)
    { (void)opaque; (void)ts; (void)entries; }

static const struct vlc_tracer_operations discard_tracer_ops =
    { .trace = vlc_tracer_discard_Trace };

static struct vlc_tracer discard_tracer =/* No-op tracer */
    { .ops = &discard_tracer_ops };

/**
 * Switchable tracer.
 *
 * A tracer that can be redirected live.
 */
struct vlc_tracer_switch {
    struct vlc_tracer *_Atomic backend;
    struct vlc_tracer frontend;
};
static const struct vlc_tracer_operations switch_ops;

static void vlc_TraceSwitch(struct vlc_tracer *tracer,
                            struct vlc_tracer *new_tracer)
{
    struct vlc_tracer_switch *traceswitch =
        container_of(tracer, struct vlc_tracer_switch, frontend);
    struct vlc_tracer *old_tracer;

    assert(tracer->ops == &switch_ops);

    if (new_tracer == NULL)
        new_tracer = &discard_tracer;

    old_tracer = atomic_exchange_explicit(&traceswitch->backend, new_tracer,
                                          memory_order_acq_rel);
    vlc_rcu_synchronize();

    if (old_tracer == new_tracer || old_tracer == NULL)
        return;

    if (old_tracer->ops != NULL && old_tracer->ops->destroy != NULL)
        old_tracer->ops->destroy(old_tracer->opaque);
}

static void vlc_tracer_switch_vaTrace(void *opaque, vlc_tick_t ts,
                                      va_list entries)
{
    struct vlc_tracer *tracer = opaque;
    assert(tracer->ops->trace == &vlc_tracer_switch_vaTrace);

    struct vlc_tracer_switch *traceswitch =
        container_of(tracer, struct vlc_tracer_switch, frontend);

    vlc_rcu_read_lock();
    struct vlc_tracer *backend =
        atomic_load_explicit(&traceswitch->backend, memory_order_acquire);
    backend->ops->trace(backend->opaque, ts, entries);
    vlc_rcu_read_unlock();
}

static void vlc_tracer_switch_Destroy(void *opaque)
{
    struct vlc_tracer *tracer = opaque;
    assert(tracer->ops->destroy == &vlc_tracer_switch_Destroy);

    vlc_TraceSwitch(tracer, &discard_tracer);

    struct vlc_tracer_switch *traceswitch =
        container_of(tracer, struct vlc_tracer_switch, frontend);
    free(traceswitch);
}

static const struct vlc_tracer_operations switch_ops = {
    vlc_tracer_switch_vaTrace,
    vlc_tracer_switch_Destroy,
};

static struct vlc_tracer *vlc_tracer_switch_Create()
{
    struct vlc_tracer_switch *traceswitch = malloc(sizeof *traceswitch);
    if (unlikely(traceswitch == NULL))
        return NULL;

    traceswitch->frontend.ops = &switch_ops;
    traceswitch->frontend.opaque = &traceswitch->frontend;
    atomic_init(&traceswitch->backend, &discard_tracer);
    return &traceswitch->frontend;
}

int vlc_TracerInit(libvlc_int_t *libvlc)
{
    libvlc_priv_t *priv = libvlc_priv(libvlc);

    /* Pre-allocate the libvlc tracer to avoid allocating it when enabling. */
    priv->libvlc_tracer = malloc(sizeof(*priv->libvlc_tracer));
    if (priv->libvlc_tracer == NULL)
        return VLC_ENOMEM;
    (*priv->libvlc_tracer) = (struct vlc_tracer){};

    struct vlc_tracer *tracerswitch = vlc_tracer_switch_Create();
    if (unlikely(tracerswitch == NULL))
    {
        FREENULL(priv->libvlc_tracer);
        return VLC_ENOMEM;
    }
    priv->tracer = tracerswitch;

    char *tracer_name = var_InheritString(libvlc, "tracer");
    struct vlc_tracer *tracer = vlc_tracer_Create(VLC_OBJECT(libvlc), tracer_name);
    free(tracer_name);
    vlc_TraceSwitch(tracerswitch, tracer);

    return VLC_SUCCESS;
}

void vlc_TracerDestroy(libvlc_int_t *libvlc)
{
    libvlc_priv_t *priv = libvlc_priv(libvlc);
    if (priv->tracer)
        vlc_tracer_Destroy(priv->tracer);

    if (priv->libvlc_tracer)
        vlc_tracer_Destroy(priv->libvlc_tracer);
    free(priv->libvlc_tracer);


}

void vlc_TraceSet(libvlc_int_t *vlc, const struct vlc_tracer_operations *ops,
                  void *opaque)
{
    libvlc_priv_t *priv = libvlc_priv(vlc);
    struct vlc_tracer *tracer = priv->tracer;

    assert(tracer != NULL);
    assert(priv->libvlc_tracer != NULL);
    priv->libvlc_tracer->ops = ops;
    priv->libvlc_tracer->opaque = opaque;
    vlc_TraceSwitch(tracer, priv->libvlc_tracer);
    atomic_store_explicit(&priv->tracer_enabled, ops != NULL,
                          memory_order_release);
}
