/*****************************************************************************
 * clock.c: Output modules synchronisation clock
 *****************************************************************************
 * Copyright (C) 2018-2019 VLC authors, VideoLAN and Videolabs SAS
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
#include <vlc_aout.h>
#include <assert.h>
#include <limits.h>
#include <vlc_tracer.h>
#include <vlc_vector.h>
#include "clock.h"
#include "clock_internal.h"

#define COEFF_THRESHOLD 0.2 /* between 0.8 and 1.2 */

struct vlc_clock_listener_id
{
    vlc_clock_t *clock;
    const struct vlc_clock_event_cbs *cbs;
    void *data;
};

struct vlc_clock_context
{
    double rate;
    double coeff;
    vlc_tick_t offset;
    vlc_tick_t first_pcr_ts;
    uint32_t clock_id;

    clock_point_t wait_sync_ref;

    struct vlc_list node;
    struct vlc_list using_clocks;
};

struct vlc_clock_main_t
{
    struct vlc_logger *logger;
    struct vlc_tracer *tracer;
    vlc_mutex_t lock;
    vlc_cond_t cond;

    vlc_clock_t *master;
    vlc_clock_t *input_master;

    unsigned rc;

    /**
     * Linear function
     * system = ts * coeff / rate + offset
     */
    clock_point_t last;
    average_t coeff_avg; /* Moving average to smooth out the instant coeff */
    vlc_tick_t delay;
    struct vlc_clock_context context;

    vlc_tick_t pause_date;

    unsigned wait_sync_ref_priority;
    clock_point_t first_pcr;
    vlc_tick_t output_dejitter; /* Delay used to absorb the output clock jitter */
    vlc_tick_t input_dejitter; /* Delay used to absorb the input jitter */

    struct VLC_VECTOR(vlc_clock_listener_id *) listeners;
    struct vlc_list prev_contexts;
};

struct vlc_clock_ops
{
    vlc_tick_t (*update)(vlc_clock_t *clock, struct vlc_clock_context *ctx,
                         vlc_tick_t system_now, vlc_tick_t ts, double rate,
                         unsigned frame_rate, unsigned frame_rate_base);
    void (*reset)(vlc_clock_t *clock);
    vlc_tick_t (*set_delay)(vlc_clock_t *clock, vlc_tick_t delay);
    vlc_tick_t (*to_system)(vlc_clock_t *clock, struct vlc_clock_context *ctx,
                            vlc_tick_t system_now, vlc_tick_t ts, double rate);
};

struct vlc_clock_t
{
    const struct vlc_clock_ops *ops;
    vlc_clock_main_t *owner;
    vlc_tick_t delay;
    unsigned priority;
    const char *track_str_id;

    const struct vlc_clock_cbs *cbs;
    void *cbs_data;

    struct vlc_list node;

    struct vlc_clock_context *context;
};

vlc_clock_listener_id *
vlc_clock_AddListener(vlc_clock_t *clock,
                      const struct vlc_clock_event_cbs *cbs,
                      void *data)
{
    vlc_clock_main_t *main_clock = clock->owner;
    assert(cbs != NULL);
    vlc_mutex_assert(&main_clock->lock);

    vlc_clock_listener_id *listener_id = malloc(sizeof(*listener_id));
    if (listener_id == NULL)
        return NULL;

    listener_id->clock = clock;
    listener_id->cbs = cbs;
    listener_id->data = data;

    bool success = vlc_vector_push(&main_clock->listeners, listener_id);
    if (!success)
    {
        free(listener_id);
        return NULL;
    }
    return listener_id;
}

void
vlc_clock_RemoveListener(vlc_clock_t *clock, vlc_clock_listener_id *listener_id)
{
    vlc_clock_main_t *main_clock = clock->owner;
    vlc_mutex_assert(&main_clock->lock);

    const vlc_clock_listener_id *it;
    vlc_vector_foreach(it, &main_clock->listeners)
        if (it == listener_id)
        {
            vlc_vector_remove(&main_clock->listeners, vlc_vector_idx_it);
            free(listener_id);
            return;
        }

    vlc_assert_unreachable();
}

#define vlc_clock_SendEvent(main_clock, event) { \
    const struct vlc_clock_listener_id *listener_id; \
    vlc_vector_foreach(listener_id, &main_clock->listeners) \
        if (listener_id->cbs->on_##event != NULL) \
            listener_id->cbs->on_##event(listener_id->data); \
}

static inline void TraceRender(struct vlc_tracer *tracer, const char *type,
                               const char *id, vlc_tick_t now, vlc_tick_t pts,
                               vlc_tick_t drift)
{
    if (now != VLC_TICK_MAX && now != VLC_TICK_INVALID)
    {
        vlc_tracer_TraceWithTs(tracer, vlc_tick_now(),
                               VLC_TRACE("type", type),
                               VLC_TRACE("id", id),
                               VLC_TRACE_TICK_NS("pts", pts),
                               VLC_TRACE_TICK_NS("render_ts", now),
                               VLC_TRACE_TICK_NS("drift", drift),
                               VLC_TRACE_END);
        vlc_tracer_TraceWithTs(tracer, now,
                               VLC_TRACE("type", type),
                               VLC_TRACE("id", id),
                               VLC_TRACE_TICK_NS("render_pts", pts),
                               VLC_TRACE_TICK_NS("drift", drift),
                               VLC_TRACE_END);

    }
    else
        vlc_tracer_Trace(tracer, VLC_TRACE("type", type),
                                 VLC_TRACE("id", id),
                                 VLC_TRACE_TICK_NS("pts", pts),
                                 VLC_TRACE_END);
}

static void context_reset(struct vlc_clock_context *ctx)
{
    ctx->coeff = 1.0f;
    ctx->rate = 1.0f;
    ctx->offset = VLC_TICK_INVALID;
    ctx->wait_sync_ref = clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);
}

static void context_init(struct vlc_clock_context *ctx)
{
    context_reset(ctx);
    ctx->clock_id = 0;
    ctx->first_pcr_ts = VLC_TICK_INVALID;
}

static vlc_tick_t context_stream_to_system(const struct vlc_clock_context *ctx,
                                           vlc_tick_t ts)
{
    if (ctx->offset == VLC_TICK_INVALID)
        return VLC_TICK_INVALID;
    return ((vlc_tick_t) (ts * ctx->coeff / ctx->rate)) + ctx->offset;
}

static void
vlc_clock_remove_current_context(vlc_clock_t *clock)
{
    struct vlc_clock_context *ctx = clock->context;
    assert(ctx != NULL);

    /* Remove the clock from the context using_clocks */
    vlc_list_remove(&clock->node);

    /* Free the context if it is not used anymore */
    if (vlc_list_is_empty(&ctx->using_clocks))
    {
        /* Remove the context from the list of previous contexts */
        vlc_list_remove(&ctx->node);
        free(ctx);
    }
    clock->context = NULL;
}

static struct vlc_clock_context *
vlc_clock_get_context(vlc_clock_t *clock, uint32_t clock_id, bool update)
{
    vlc_clock_main_t *main_clock = clock->owner;

    if (vlc_list_is_empty(&main_clock->prev_contexts)
     || clock_id == VLC_CLOCK_ID_LAST)
        return &main_clock->context;

    struct vlc_clock_context *ctx = NULL, *ctx_it = NULL;
    vlc_list_foreach(ctx_it, &main_clock->prev_contexts, node)
        if (ctx_it->clock_id == clock_id)
        {
            ctx = ctx_it;
            break;
        }

    if (ctx == NULL) /* Using the main context */
    {
        ctx = &main_clock->context;
        assert(ctx->clock_id == clock_id);
        if (clock->context != NULL && update)
            vlc_clock_remove_current_context(clock);
    }
    else
    {
        if (clock->context != ctx)
        {
            if (clock->context != NULL)
                vlc_clock_remove_current_context(clock);
            vlc_list_append(&clock->node, &ctx->using_clocks);
            clock->context = ctx;
        }
    }

    return ctx;
}

static void vlc_clock_main_reset(vlc_clock_main_t *main_clock)
{
    struct vlc_clock_context *ctx = &main_clock->context;

    context_reset(ctx);
    AvgResetAndFill(&main_clock->coeff_avg, ctx->coeff);

    main_clock->wait_sync_ref_priority = UINT_MAX;
    main_clock->last = clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);
    vlc_cond_broadcast(&main_clock->cond);
}

static inline void vlc_clock_on_update(vlc_clock_t *clock,
                                       vlc_tick_t system_now,
                                       vlc_tick_t ts,
                                       vlc_tick_t drift,
                                       double rate,
                                       unsigned frame_rate,
                                       unsigned frame_rate_base)
{
    vlc_clock_main_t *main_clock = clock->owner;
    if (clock->cbs)
        clock->cbs->on_update(system_now, ts, rate, frame_rate, frame_rate_base,
                              clock->cbs_data);

    if (main_clock->tracer != NULL && clock->track_str_id != NULL &&
        system_now != VLC_TICK_INVALID && system_now != VLC_TICK_MAX)
        TraceRender(main_clock->tracer, "RENDER", clock->track_str_id,
                    system_now, ts, drift);
}

static vlc_tick_t vlc_clock_master_update(vlc_clock_t *clock,
                                          struct vlc_clock_context *ctx,
                                          vlc_tick_t system_now,
                                          vlc_tick_t ts, double rate,
                                          unsigned frame_rate,
                                          unsigned frame_rate_base)
{
    vlc_clock_main_t *main_clock = clock->owner;

    if (unlikely(ts == VLC_TICK_INVALID || system_now == VLC_TICK_INVALID))
        return VLC_TICK_INVALID;

    if (ctx != &main_clock->context)
        return VLC_TICK_INVALID; /* Don't update previous contexts */

    /* If system_now is VLC_TICK_MAX, the update is forced, don't modify
     * anything but only notify the new clock point. */
    if (system_now != VLC_TICK_MAX)
    {
        if (ctx->offset != VLC_TICK_INVALID
         && ts != main_clock->last.stream)
        {
            if (rate == ctx->rate)
            {
                /* We have a reference so we can update coeff */
                vlc_tick_t system_diff = system_now - main_clock->last.system;
                vlc_tick_t stream_diff = ts - main_clock->last.stream;

                double instant_coeff = system_diff / (double) stream_diff * rate;

                /* System and stream ts should be incrementing */
                bool decreasing_ts = system_diff < 0 || stream_diff < 0;
                /* The instant coeff should always be around 1.0 */
                bool coefficient_unstable = instant_coeff > 1.0 + COEFF_THRESHOLD
                    || instant_coeff < 1.0 - COEFF_THRESHOLD;

                if (decreasing_ts || coefficient_unstable)
                {
                    if (main_clock->logger != NULL)
                    {
                        if (decreasing_ts)
                            vlc_warning(main_clock->logger, "resetting master clock: "
                                        "decreasing ts: system: %"PRId64 ", stream: %" PRId64,
                                        system_diff, stream_diff);
                        else
                            vlc_warning(main_clock->logger, "resetting master clock: "
                                        "coefficient too unstable: %f", instant_coeff);
                    }

                    if (main_clock->tracer != NULL && clock->track_str_id != NULL)
                        vlc_tracer_TraceEvent(main_clock->tracer, "RENDER",
                                              clock->track_str_id,
                                              "reset_bad_source");

                    vlc_clock_SendEvent(main_clock, discontinuity);

                    /* Reset and continue (calculate the offset from the
                     * current point) */
                    vlc_clock_main_reset(main_clock);
                }
                else
                {
                    AvgUpdate(&main_clock->coeff_avg, instant_coeff);
                    ctx->coeff = AvgGet(&main_clock->coeff_avg);
                }
            }
        }
        else
        {
            main_clock->wait_sync_ref_priority = UINT_MAX;
            ctx->wait_sync_ref =
                clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);

            vlc_clock_SendEvent(main_clock, discontinuity);
        }

        ctx->offset = system_now - ((vlc_tick_t) (ts * ctx->coeff / rate));

        if (main_clock->tracer != NULL && clock->track_str_id != NULL)
            vlc_tracer_Trace(main_clock->tracer,
                             VLC_TRACE("type", "RENDER"),
                             VLC_TRACE("id", clock->track_str_id),
                             VLC_TRACE_TICK_NS("offset", ctx->offset),
                             VLC_TRACE("coeff", ctx->coeff),
                             VLC_TRACE_END);

        main_clock->last = clock_point_Create(system_now, ts);

        ctx->rate = rate;
        vlc_cond_broadcast(&main_clock->cond);
    }

    /* Fix the reported ts if both master and slaves source are delayed. This
     * happens if we apply a positive delay to the master, and then lower it. */
    if (clock->delay > 0 && main_clock->delay < 0 && ts > -main_clock->delay)
        ts += main_clock->delay;

    vlc_tick_t drift = VLC_TICK_INVALID;
    vlc_clock_on_update(clock, system_now, ts, drift,
                        rate, frame_rate, frame_rate_base);
    return drift;
}

static void vlc_clock_master_reset(vlc_clock_t *clock)
{
    vlc_clock_main_t *main_clock = clock->owner;

    if (main_clock->tracer != NULL && clock->track_str_id != NULL)
        vlc_tracer_TraceEvent(main_clock->tracer, "RENDER", clock->track_str_id,
                              "reset_user");
    vlc_clock_main_reset(main_clock);

    assert(main_clock->delay <= 0);
    assert(clock->delay >= 0);
    /* Move the delay from the slaves to the master */
    if (clock->delay != 0 && main_clock->delay != 0)
    {
        vlc_tick_t delta = clock->delay + main_clock->delay;
        if (delta > 0)
        {
            clock->delay = delta;
            main_clock->delay = 0;
        }
        else
        {
            clock->delay = 0;
            main_clock->delay = delta;
        }
    }

    vlc_clock_on_update(clock, VLC_TICK_INVALID, VLC_TICK_INVALID, VLC_TICK_INVALID, 1.f, 0, 0);
}

static vlc_tick_t vlc_clock_master_set_delay(vlc_clock_t *clock, vlc_tick_t delay)
{
    vlc_clock_main_t *main_clock = clock->owner;

    vlc_tick_t delta = delay - clock->delay;

    if (delta > 0)
    {
        /* The master clock is delayed */
        main_clock->delay = 0;
        clock->delay = delay;
    }
    else
    {
        /* Delay all slaves clock instead of advancing the master one */
        main_clock->delay = delta;
    }

    assert(main_clock->delay <= 0);
    assert(clock->delay >= 0);

    vlc_cond_broadcast(&main_clock->cond);
    return delta;
}

static vlc_tick_t
vlc_clock_monotonic_to_system(vlc_clock_t *clock, struct vlc_clock_context *ctx,
                              vlc_tick_t now, vlc_tick_t ts, double rate)
{
    vlc_clock_main_t *main_clock = clock->owner;

    if (clock->priority < main_clock->wait_sync_ref_priority
     && ctx == &main_clock->context)
    {
        /* XXX: This input_delay calculation is needed until we (finally) get
         * ride of the input clock. This code is adapted from input_clock.c and
         * is used to introduce the same delay than the input clock (first PTS
         * - first PCR). */
        vlc_tick_t pcr_delay =
            main_clock->first_pcr.system == VLC_TICK_INVALID ? 0 :
            (ts - main_clock->first_pcr.stream) / rate +
            main_clock->first_pcr.system - now;

        if (pcr_delay > CR_MAX_GAP)
        {
            if (main_clock->logger != NULL)
                vlc_error(main_clock->logger, "Invalid PCR delay ! Ignoring it...");
            pcr_delay = 0;
        }

        const vlc_tick_t input_delay = main_clock->input_dejitter + pcr_delay;

        const vlc_tick_t delay =
            __MAX(input_delay, main_clock->output_dejitter);

        main_clock->wait_sync_ref_priority = clock->priority;
        ctx->wait_sync_ref = clock_point_Create(now + delay, ts);
    }

    return (ts - ctx->wait_sync_ref.stream) / rate + ctx->wait_sync_ref.system;
}

static vlc_tick_t vlc_clock_slave_to_system(vlc_clock_t *clock,
                                            struct vlc_clock_context *ctx,
                                            vlc_tick_t now, vlc_tick_t ts,
                                            double rate)
{
    vlc_clock_main_t *main_clock = clock->owner;

    vlc_tick_t system = context_stream_to_system(ctx, ts);
    if (system == VLC_TICK_INVALID)
    {
        /* We don't have a master sync point, let's fallback to a monotonic ref
         * point */
        system = vlc_clock_monotonic_to_system(clock, ctx, now, ts, rate);
    }

    return system + (clock->delay - main_clock->delay) * rate;
}

static vlc_tick_t vlc_clock_master_to_system(vlc_clock_t *clock,
                                             struct vlc_clock_context *ctx,
                                             vlc_tick_t now, vlc_tick_t ts,
                                             double rate)
{
    vlc_tick_t system = context_stream_to_system(ctx, ts);
    if (system == VLC_TICK_INVALID)
    {
        /* We don't have a master sync point, let's fallback to a monotonic ref
         * point */
        system = vlc_clock_monotonic_to_system(clock, ctx, now, ts, rate);
    }

    return system;
}

static vlc_tick_t vlc_clock_slave_update(vlc_clock_t *clock,
                                         struct vlc_clock_context *ctx,
                                         vlc_tick_t system_now,
                                         vlc_tick_t ts, double rate,
                                         unsigned frame_rate,
                                         unsigned frame_rate_base)
{
    if (system_now == VLC_TICK_MAX)
    {
        /* If system_now is VLC_TICK_MAX, the update is forced, don't modify
         * anything but only notify the new clock point. */
        vlc_clock_on_update(clock, VLC_TICK_MAX, ts, VLC_TICK_INVALID,
                            rate, frame_rate, frame_rate_base);
        return VLC_TICK_MAX;
    }

    vlc_tick_t computed = clock->ops->to_system(clock, ctx, system_now, ts, rate);

    vlc_tick_t drift = computed - system_now;
    vlc_clock_on_update(clock, computed, ts, drift, rate,
                        frame_rate, frame_rate_base);
    return drift;
}

static void vlc_clock_slave_reset(vlc_clock_t *clock)
{
    vlc_clock_main_t *main_clock = clock->owner;
    struct vlc_clock_context *ctx = &main_clock->context;

    main_clock->wait_sync_ref_priority = UINT_MAX;
    ctx->wait_sync_ref = clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);

    vlc_clock_on_update(clock, VLC_TICK_INVALID, VLC_TICK_INVALID,
                        VLC_TICK_INVALID, 1.0f, 0, 0);
}

static vlc_tick_t vlc_clock_slave_set_delay(vlc_clock_t *clock, vlc_tick_t delay)
{
    vlc_clock_main_t *main_clock = clock->owner;

    clock->delay = delay;

    vlc_cond_broadcast(&main_clock->cond);
    return 0;
}

void vlc_clock_Lock(vlc_clock_t *clock)
{
    vlc_clock_main_t *main_clock = clock->owner;
    vlc_mutex_lock(&main_clock->lock);
}

void vlc_clock_Unlock(vlc_clock_t *clock)
{
    vlc_clock_main_t *main_clock = clock->owner;
    vlc_mutex_unlock(&main_clock->lock);
}

static inline void AssertLocked(vlc_clock_t *clock)
{
    vlc_clock_main_t *main_clock = clock->owner;
    vlc_mutex_assert(&main_clock->lock);
}

bool vlc_clock_IsPaused(vlc_clock_t *clock)
{
    AssertLocked(clock);

    vlc_clock_main_t *main_clock = clock->owner;
    return main_clock->pause_date != VLC_TICK_INVALID;
}

int vlc_clock_Wait(vlc_clock_t *clock, vlc_tick_t deadline)
{
    AssertLocked(clock);

    vlc_clock_main_t *main_clock = clock->owner;
    return vlc_cond_timedwait(&main_clock->cond, &main_clock->lock, deadline);
}

void vlc_clock_Wake(vlc_clock_t *clock)
{
    AssertLocked(clock);

    vlc_clock_main_t *main_clock = clock->owner;
    vlc_cond_broadcast(&main_clock->cond);
}

vlc_clock_main_t *vlc_clock_main_New(struct vlc_logger *parent_logger, struct vlc_tracer *parent_tracer)
{
    vlc_clock_main_t *main_clock = malloc(sizeof(vlc_clock_main_t));

    if (main_clock == NULL)
        return NULL;

    struct vlc_clock_context *ctx = &main_clock->context;
    main_clock->logger = vlc_LogHeaderCreate(parent_logger, "clock");
    main_clock->tracer = parent_tracer;

    vlc_mutex_init(&main_clock->lock);
    vlc_cond_init(&main_clock->cond);
    main_clock->input_master = main_clock->master = NULL;
    main_clock->rc = 1;

    context_init(ctx);
    main_clock->delay = 0;

    main_clock->first_pcr =
        clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);
    main_clock->wait_sync_ref_priority = UINT_MAX;
    main_clock->last = clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);

    main_clock->pause_date = VLC_TICK_INVALID;
    main_clock->input_dejitter = DEFAULT_PTS_DELAY;
    main_clock->output_dejitter = AOUT_MAX_PTS_ADVANCE * 2;

    AvgInit(&main_clock->coeff_avg, 10);
    AvgResetAndFill(&main_clock->coeff_avg, ctx->coeff);

    vlc_vector_init(&main_clock->listeners);
    vlc_list_init(&main_clock->prev_contexts);

    return main_clock;
}

void vlc_clock_main_Reset(vlc_clock_main_t *main_clock)
{
    vlc_mutex_assert(&main_clock->lock);

    vlc_clock_main_reset(main_clock);
    main_clock->first_pcr =
        clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);
}

uint32_t vlc_clock_main_SetFirstPcr(vlc_clock_main_t *main_clock,
                                    vlc_tick_t system_now, vlc_tick_t ts)
{
    vlc_mutex_assert(&main_clock->lock);
    struct vlc_clock_context *ctx = &main_clock->context;

    if (main_clock->first_pcr.system != VLC_TICK_INVALID)
    {
        struct vlc_clock_context *dupctx = malloc(sizeof(*dupctx));
        if (dupctx != NULL)
        {
            *dupctx = *ctx;
            vlc_list_init(&dupctx->using_clocks);
            vlc_list_append(&dupctx->node, &main_clock->prev_contexts);
        }
        ctx->clock_id++;
        if (unlikely(ctx->clock_id == VLC_CLOCK_ID_LAST))
            ctx->clock_id = 0;
    }
    vlc_clock_main_reset(main_clock);
    ctx->first_pcr_ts = ts;

    main_clock->first_pcr = clock_point_Create(system_now, ts);
    main_clock->wait_sync_ref_priority = UINT_MAX;
    ctx->wait_sync_ref =
        clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);

    return ctx->clock_id;
}

void vlc_clock_main_SetInputDejitter(vlc_clock_main_t *main_clock,
                                     vlc_tick_t delay)
{
    vlc_mutex_assert(&main_clock->lock);

    main_clock->input_dejitter = delay;
}

void vlc_clock_main_SetDejitter(vlc_clock_main_t *main_clock,
                                vlc_tick_t dejitter)
{
    vlc_mutex_assert(&main_clock->lock);

    main_clock->output_dejitter = dejitter;
}

void vlc_clock_main_ChangePause(vlc_clock_main_t *main_clock, vlc_tick_t now,
                                bool paused)
{
    vlc_mutex_assert(&main_clock->lock);

    assert(paused == (main_clock->pause_date == VLC_TICK_INVALID));

    if (paused)
        main_clock->pause_date = now;
    else
    {
        struct vlc_clock_context *ctx = &main_clock->context;
        /**
         * Only apply a delay if the clock has a reference point to avoid
         * messing up the timings if the stream was paused then seeked
         */
        const vlc_tick_t delay = now - main_clock->pause_date;
        if (ctx->offset != VLC_TICK_INVALID)
        {
            main_clock->last.system += delay;
            ctx->offset += delay;
        }
        if (main_clock->first_pcr.system != VLC_TICK_INVALID)
            main_clock->first_pcr.system += delay;
        if (ctx->wait_sync_ref.system != VLC_TICK_INVALID)
            ctx->wait_sync_ref.system += delay;
        main_clock->pause_date = VLC_TICK_INVALID;
        vlc_cond_broadcast(&main_clock->cond);
    }
}

void vlc_clock_main_Delete(vlc_clock_main_t *main_clock)
{
    assert(main_clock->rc == 1);
    if (main_clock->logger != NULL)
        vlc_LogDestroy(main_clock->logger);

    assert(main_clock->listeners.size == 0);
    vlc_vector_destroy(&main_clock->listeners);

    struct vlc_clock_context *ctx_it;
    vlc_list_foreach(ctx_it, &main_clock->prev_contexts, node)
        free(ctx_it);

    free(main_clock);
}

void vlc_clock_main_Lock(vlc_clock_main_t *main_clock)
{
    vlc_mutex_lock(&main_clock->lock);
}

void vlc_clock_main_Unlock(vlc_clock_main_t *main_clock)
{
    vlc_mutex_assert(&main_clock->lock);
    vlc_mutex_unlock(&main_clock->lock);
}

vlc_tick_t vlc_clock_Update(vlc_clock_t *clock, uint32_t clock_id,
                            vlc_tick_t system_now, vlc_tick_t ts,
                            double rate)
{
    AssertLocked(clock);
    struct vlc_clock_context *ctx = vlc_clock_get_context(clock, clock_id, true);

    return clock->ops->update(clock, ctx, system_now, ts, rate, 0, 0);
}

vlc_tick_t vlc_clock_UpdateVideo(vlc_clock_t *clock, uint32_t clock_id,
                                 vlc_tick_t system_now, vlc_tick_t ts,
                                 double rate,
                                 unsigned frame_rate, unsigned frame_rate_base)
{
    AssertLocked(clock);
    struct vlc_clock_context *ctx = vlc_clock_get_context(clock, clock_id, true);

    return clock->ops->update(clock, ctx, system_now, ts, rate,
                              frame_rate, frame_rate_base);
}

void vlc_clock_Reset(vlc_clock_t *clock)
{
    AssertLocked(clock);
    clock->ops->reset(clock);
}

vlc_tick_t vlc_clock_SetDelay(vlc_clock_t *clock, vlc_tick_t delay)
{
    AssertLocked(clock);
    return clock->ops->set_delay(clock, delay);
}

vlc_tick_t vlc_clock_ConvertToSystem(vlc_clock_t *clock, uint32_t clock_id,
                                     vlc_tick_t system_now, vlc_tick_t ts,
                                     double rate)
{
    AssertLocked(clock);
    struct vlc_clock_context *ctx = vlc_clock_get_context(clock, clock_id, false);

    return clock->ops->to_system(clock, ctx, system_now, ts, rate);
}

static const struct vlc_clock_ops master_ops = {
    .update = vlc_clock_master_update,
    .reset = vlc_clock_master_reset,
    .set_delay = vlc_clock_master_set_delay,
    .to_system = vlc_clock_master_to_system,
};

static const struct vlc_clock_ops slave_ops = {
    .update = vlc_clock_slave_update,
    .reset = vlc_clock_slave_reset,
    .set_delay = vlc_clock_slave_set_delay,
    .to_system = vlc_clock_slave_to_system,
};

static vlc_clock_t *vlc_clock_main_Create(vlc_clock_main_t *main_clock,
                                          const char* track_str_id,
                                          unsigned priority,
                                          const struct vlc_clock_cbs *cbs,
                                          void *cbs_data)
{
    vlc_clock_t *clock = malloc(sizeof(vlc_clock_t));
    if (clock == NULL)
        return NULL;

    clock->owner = main_clock;
    clock->track_str_id = track_str_id;
    clock->delay = 0;
    clock->cbs = cbs;
    clock->cbs_data = cbs_data;
    clock->priority = priority;
    assert(!cbs || cbs->on_update);

    clock->context = NULL;

    return clock;
}

vlc_clock_t *vlc_clock_main_CreateMaster(vlc_clock_main_t *main_clock,
                                         const char *track_str_id,
                                         const struct vlc_clock_cbs *cbs,
                                         void *cbs_data)
{
    vlc_mutex_assert(&main_clock->lock);

    /* The master has always the 0 priority */
    vlc_clock_t *clock = vlc_clock_main_Create(main_clock, track_str_id, 0, cbs, cbs_data);
    if (!clock)
        return NULL;

    assert(main_clock->master == NULL);

    if (main_clock->input_master == NULL)
        clock->ops = &master_ops;
    else
        clock->ops = &slave_ops;

    main_clock->master = clock;
    main_clock->rc++;

    return clock;
}

vlc_clock_t *vlc_clock_main_CreateInputMaster(vlc_clock_main_t *main_clock)
{
    vlc_mutex_assert(&main_clock->lock);

    /* The master has always the 0 priority */
    vlc_clock_t *clock = vlc_clock_main_Create(main_clock, "input", 0, NULL, NULL);
    if (!clock)
        return NULL;

    assert(main_clock->input_master == NULL);

    /* Even if the master ES clock has already been created, it should not
     * have updated any points */
    assert(main_clock->context.offset == VLC_TICK_INVALID);

    /* Override the master ES clock if it exists */
    if (main_clock->master != NULL)
        main_clock->master->ops = &slave_ops;

    clock->ops = &master_ops;
    main_clock->input_master = clock;
    main_clock->rc++;

    return clock;
}

vlc_clock_t *vlc_clock_main_CreateSlave(vlc_clock_main_t *main_clock,
                                        const char* track_str_id,
                                        enum es_format_category_e cat,
                                        const struct vlc_clock_cbs *cbs,
                                        void *cbs_data)
{
    vlc_mutex_assert(&main_clock->lock);

    /* SPU outputs should have lower priority than VIDEO outputs since they
     * necessarily depend on a VIDEO output. This mean that a SPU reference
     * point will always be overridden by AUDIO or VIDEO outputs. Cf.
     * vlc_clock_monotonic_to_system */
    unsigned priority;
    switch (cat)
    {
        case VIDEO_ES:
        case AUDIO_ES:
            priority = 1;
            break;
        case SPU_ES:
        default:
            priority = 2;
            break;
    }

    vlc_clock_t *clock = vlc_clock_main_Create(main_clock, track_str_id, priority, cbs,
                                               cbs_data);
    if (!clock)
        return NULL;

    clock->ops = &slave_ops;
    main_clock->rc++;

    return clock;
}

vlc_clock_t *vlc_clock_CreateSlave(const vlc_clock_t *clock,
                                   enum es_format_category_e cat)
{
    return vlc_clock_main_CreateSlave(clock->owner, clock->track_str_id, cat, NULL, NULL);
}

void vlc_clock_Delete(vlc_clock_t *clock)
{
    vlc_clock_main_t *main_clock = clock->owner;
    vlc_mutex_lock(&main_clock->lock);

    if (clock->context != NULL)
        vlc_clock_remove_current_context(clock);

    if (clock == main_clock->input_master)
    {
        /* The input master must be the last clock to be deleted */
        assert(main_clock->rc == 2);
    }
    else if (clock == main_clock->master)
    {
        /* Don't reset the main clock if the master has been overridden by the
         * input master */
        if (main_clock->input_master != NULL)
            vlc_clock_main_reset(main_clock);
        main_clock->master = NULL;
    }
    main_clock->rc--;
    vlc_mutex_unlock(&main_clock->lock);
    free(clock);
}
