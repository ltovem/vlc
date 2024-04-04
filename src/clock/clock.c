/*****************************************************************************
 * clock.c: Output modules synchronisation clock
 *****************************************************************************
 * Copyright (C) 2018-2019 VLC authors, VideoLAN and Videolabs SAS
 * Copyright (C) 2024      Videolabs
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
    double rate;
    double coeff;
    vlc_tick_t offset;
    vlc_tick_t delay;

    vlc_tick_t pause_date;

    unsigned wait_sync_ref_priority;
    clock_point_t wait_sync_ref; /* When the master */
    clock_point_t first_pcr;

    /**
     * Start point emitted by the buffering to indicate when we supposedly
     * started the playback. It does not account for latency of the output
     * when one of them is driving the main_clock. */
    clock_point_t start_time;

    vlc_tick_t output_dejitter; /* Delay used to absorb the output clock jitter */
    vlc_tick_t input_dejitter; /* Delay used to absorb the input jitter */

    struct VLC_VECTOR(vlc_clock_listener_id *) listeners;
};

struct vlc_clock_ops
{
    vlc_tick_t (*update)(vlc_clock_t *clock, vlc_tick_t system_now,
                         vlc_tick_t ts, double rate,
                         unsigned frame_rate, unsigned frame_rate_base);
    void (*reset)(vlc_clock_t *clock);
    vlc_tick_t (*set_delay)(vlc_clock_t *clock, vlc_tick_t delay);
    vlc_tick_t (*to_system)(vlc_clock_t *clock, vlc_tick_t system_now,
                            vlc_tick_t ts, double rate);

    /**
     * Signal the clock bus that this clock is ready to be started.
     *
     * \param clock         the clock which is ready to start
     * \param system_now    the system time for the start point
     * \param ts            the time for the start point
     **/
    void (*start)(vlc_clock_t *clock, vlc_tick_t system_now, vlc_tick_t ts);
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

static vlc_tick_t main_stream_to_system(vlc_clock_main_t *main_clock,
                                        vlc_tick_t ts)
{
    if (main_clock->last.system == VLC_TICK_INVALID)
        return VLC_TICK_INVALID;
    return ((vlc_tick_t) ((ts - main_clock->start_time.stream) * main_clock->coeff / main_clock->rate))
            + main_clock->offset
            + main_clock->start_time.system;
}

static void vlc_clock_main_reset(vlc_clock_main_t *main_clock)
{
    main_clock->coeff = 1.0f;
    main_clock->rate = 1.0f;
    AvgResetAndFill(&main_clock->coeff_avg, main_clock->coeff);
    main_clock->offset = VLC_TICK_INVALID;

    main_clock->wait_sync_ref_priority = UINT_MAX;
    main_clock->wait_sync_ref
        = main_clock->first_pcr
        = main_clock->last
        = clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);

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


static void vlc_clock_master_update_coeff(
    vlc_clock_t *clock, vlc_tick_t system_now, vlc_tick_t ts, double rate)
{
    vlc_clock_main_t *main_clock = clock->owner;
    vlc_mutex_assert(&main_clock->lock);

    if (main_clock->offset != VLC_TICK_INVALID
     && ts != main_clock->last.stream)
    {
        if (rate == main_clock->rate)
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
                main_clock->coeff = 1.f;
                AvgResetAndFill(&main_clock->coeff_avg, 1.);
                main_clock->offset = system_now
                    - ((vlc_tick_t) ((ts - main_clock->start_time.stream) * main_clock->coeff / rate))
                    - main_clock->start_time.system;
            }
            else
            {
                AvgUpdate(&main_clock->coeff_avg, instant_coeff);
                main_clock->coeff = AvgGet(&main_clock->coeff_avg);
            }
        }
    }
    else
    {
        main_clock->wait_sync_ref_priority = UINT_MAX;
        main_clock->wait_sync_ref =
            clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);

        vlc_clock_SendEvent(main_clock, discontinuity);
    }

    main_clock->offset = system_now
        - ((vlc_tick_t) ((ts - main_clock->start_time.stream) * main_clock->coeff / rate))
        - main_clock->start_time.system;

    if (main_clock->tracer != NULL && clock->track_str_id != NULL)
        vlc_tracer_Trace(main_clock->tracer,
                         VLC_TRACE("type", "RENDER"),
                         VLC_TRACE("id", clock->track_str_id),
                         VLC_TRACE_TICK_NS("offset", main_clock->offset),
                         VLC_TRACE("coeff", main_clock->coeff),
                         VLC_TRACE_END);

    main_clock->rate = rate;
    vlc_cond_broadcast(&main_clock->cond);
}

static vlc_tick_t vlc_clock_master_update(vlc_clock_t *clock,
                                          vlc_tick_t system_now,
                                          vlc_tick_t ts, double rate,
                                          unsigned frame_rate,
                                          unsigned frame_rate_base)
{
    vlc_clock_main_t *main_clock = clock->owner;

    if (unlikely(ts == VLC_TICK_INVALID || system_now == VLC_TICK_INVALID))
        return VLC_TICK_INVALID;

    /* If system_now is VLC_TICK_MAX, the update is forced, don't modify
     * anything but only notify the new clock point. */
    if (system_now != VLC_TICK_MAX)
        vlc_clock_master_update_coeff(clock, system_now, ts, rate);

    main_clock->last = clock_point_Create(system_now, ts);

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

static void
vlc_clock_input_start(vlc_clock_t *clock,
                      vlc_tick_t start_date,
                      vlc_tick_t first_ts)
{
    vlc_clock_main_t *main_clock = clock->owner;
    vlc_mutex_assert(&main_clock->lock);

    /* vlc_clock_Start can only be called once after a clock reset. */
    assert (main_clock->start_time.system == VLC_TICK_INVALID);
    assert (main_clock->start_time.stream == VLC_TICK_INVALID);

    main_clock->start_time = clock_point_Create(start_date, first_ts);
    main_clock->offset = 0;
    main_clock->wait_sync_ref_priority = UINT_MAX;
    main_clock->wait_sync_ref =
        clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);

    if (main_clock->tracer != NULL)
        vlc_tracer_TraceEvent(main_clock->tracer, "clock", clock->track_str_id, "start");
}

static vlc_tick_t
vlc_clock_monotonic_to_system(vlc_clock_t *clock, vlc_tick_t now,
                              vlc_tick_t ts, double rate)
{
    vlc_clock_main_t *main_clock = clock->owner;

    if (clock->priority < main_clock->wait_sync_ref_priority)
    {
        /* XXX: This input_delay calculation is needed until we (finally) get
         * ride of the input clock. This code is adapted from input_clock.c and
         * is used to introduce the same delay than the input clock (first PTS
         * - first PCR). */
        vlc_tick_t pcr_delay = 0;
        if (main_clock->start_time.system != VLC_TICK_INVALID)
            pcr_delay = (ts - main_clock->start_time.stream) / rate +
                main_clock->start_time.system - now;
        else if (main_clock->first_pcr.system != VLC_TICK_INVALID)
            pcr_delay = (ts - main_clock->first_pcr.stream) / rate +
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
        main_clock->wait_sync_ref = clock_point_Create(now + delay, ts);
    }
    return (ts - main_clock->wait_sync_ref.stream) / rate
        + main_clock->wait_sync_ref.system;
}

static vlc_tick_t vlc_clock_slave_to_system(vlc_clock_t *clock,
                                            vlc_tick_t now, vlc_tick_t ts,
                                            double rate)
{
    vlc_clock_main_t *main_clock = clock->owner;

    vlc_tick_t system = main_stream_to_system(main_clock, ts);
    if (system == VLC_TICK_INVALID)
    {
        /* We don't have a master sync point, let's fallback to a monotonic ref
         * point */
        system = vlc_clock_monotonic_to_system(clock, now, ts, rate);
    }

    return system + (clock->delay - main_clock->delay) * rate;
}

static vlc_tick_t vlc_clock_master_to_system(vlc_clock_t *clock,
                                             vlc_tick_t now, vlc_tick_t ts,
                                             double rate)
{
    vlc_clock_main_t *main_clock = clock->owner;

    vlc_tick_t system = main_stream_to_system(main_clock, ts);
    if (system == VLC_TICK_INVALID)
    {
        /* We don't have a master sync point, let's fallback to a monotonic ref
         * point */
        system = vlc_clock_monotonic_to_system(clock, now, ts, rate);
    }

    return system;
}

static vlc_tick_t vlc_clock_slave_update(vlc_clock_t *clock,
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

    vlc_tick_t computed = clock->ops->to_system(clock, system_now, ts, rate);

    vlc_tick_t drift = computed - system_now;
    vlc_clock_on_update(clock, computed, ts, drift, rate,
                        frame_rate, frame_rate_base);
    return drift;
}

static void vlc_clock_slave_reset(vlc_clock_t *clock)
{
    vlc_clock_main_t *main_clock = clock->owner;
    main_clock->wait_sync_ref_priority = UINT_MAX;
    main_clock->wait_sync_ref =
        clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);

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

static void
vlc_clock_output_start(vlc_clock_t *clock,
                       vlc_tick_t start_date,
                       vlc_tick_t first_ts)
{
    (void)start_date; (void)first_ts;

    vlc_clock_main_t *main_clock = clock->owner;
    vlc_mutex_assert(&main_clock->lock);

#if 0
    /* Disabled for now, the handling will be done in later commit. */
    /* vlc_clock_Start must have already been called. */
    assert (main_clock->start_time.system != VLC_TICK_INVALID);
    assert (main_clock->start_time.stream != VLC_TICK_INVALID);
#endif
    if (main_clock->start_time.system == VLC_TICK_INVALID)
        return;

    if (clock->priority >= main_clock->wait_sync_ref_priority)
        goto end;

    /**
     * The clock should have already been started, so we have a valid
     * start_time which links a PCR value to a time where the PCR is being
     * "used". Any PTS from the output will necessarily be converted after
     * this start_time, and will necessarily have bigger PTS than the
     * registered PCR, so we can use the difference to find how much delta
     * there is between the first PTS and the first PCR.
     *
     * When setting up the outputs, some amount of wait will already be
     * consumed, for instance when playing live stream, we consume some time
     * between the first decoded packet and the end of the buffering. The
     * core currently handles this by substracting the buffering duration to
     * the start time, and we re-inject this duration through the pts-delay,
     * called input_dejitter in the clock.
     *
     *    |  Start                   End of      Decoders ready
     *    |  buffering               buffering   (DecoderWaitUnblock)
     *    |     v                       v           v
     *    |     x-----------------------x-----------x---> system time
     *    |
     *    |                               Preroll and
     *    |             pts delay         decoder delay
     *    |     |----------------------->|---------->
     *    |                 <-----------------------|
     *    |                     Bufering duration
     *    |                       (media time)
     *    |
     *    | Fig.1: System time representation of the clock startup
     *
     * The defined start_time from the input is set according to the PCR
     * but the first PTS received by an output is likely later than this
     * clock time, so the real output start_time must be shifted to keep
     * intra-media synchronization in acceptable levels.
     *
     *    |    Start     First audio    First video
     *    |    time      packet PTS     packet PTS
     *    |      v            v            v
     *    |      x------------x------------x----------> media time
     *    |
     *    |      |------------>
     *    |      Audio PCR delay
     *    |
     *    |      |------------------------->
     *    |      Video PCR delay
     *    |
     *    | Fig.2: Media time representation of the clock startup
     *
     * If the pts-delay is very low, and if the PCR delay is very small,
     * an additional output_dejitter is used to offset the beginning of
     * the playback in a uniform manner, to let some time for the outputs
     * to start. It only makes sense when one of the output will drive
     * the bus clock.
     **/

    vlc_tick_t pcr_delay = (first_ts - main_clock->start_time.stream) / main_clock->rate
                         + main_clock->start_time.system - start_date;

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
    main_clock->wait_sync_ref = clock_point_Create(start_date + delay, first_ts);

end:
    if (main_clock->tracer != NULL)
        vlc_tracer_TraceEvent(main_clock->tracer, "clock", clock->track_str_id, "start");
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

    main_clock->logger = vlc_LogHeaderCreate(parent_logger, "clock");
    main_clock->tracer = parent_tracer;

    vlc_mutex_init(&main_clock->lock);
    vlc_cond_init(&main_clock->cond);
    main_clock->input_master = main_clock->master = NULL;
    main_clock->rc = 1;

    main_clock->coeff = 1.0f;
    main_clock->rate = 1.0f;
    main_clock->offset = VLC_TICK_INVALID;
    main_clock->delay = 0;

    main_clock->first_pcr =
        clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);
    main_clock->wait_sync_ref_priority = UINT_MAX;
    main_clock->wait_sync_ref = main_clock->last =
        clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);

    main_clock->start_time
        = clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);

    main_clock->pause_date = VLC_TICK_INVALID;
    main_clock->input_dejitter = DEFAULT_PTS_DELAY;
    main_clock->output_dejitter = AOUT_MAX_PTS_ADVANCE * 2;

    AvgInit(&main_clock->coeff_avg, 10);
    AvgResetAndFill(&main_clock->coeff_avg, main_clock->coeff);

    vlc_vector_init(&main_clock->listeners);

    return main_clock;
}

void vlc_clock_main_Reset(vlc_clock_main_t *main_clock)
{
    vlc_mutex_assert(&main_clock->lock);

    vlc_clock_main_reset(main_clock);
    main_clock->first_pcr =
        clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);

    main_clock->start_time =
        clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);
}

void vlc_clock_main_SetFirstPcr(vlc_clock_main_t *main_clock,
                                vlc_tick_t system_now, vlc_tick_t ts)
{
    vlc_mutex_assert(&main_clock->lock);

    if (main_clock->first_pcr.system == VLC_TICK_INVALID)
    {
        main_clock->first_pcr = clock_point_Create(system_now, ts);
        main_clock->wait_sync_ref_priority = UINT_MAX;
        main_clock->wait_sync_ref =
            clock_point_Create(VLC_TICK_INVALID, VLC_TICK_INVALID);
    }
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
    {
        main_clock->pause_date = now;
        return;
    }

    /**
     * Only apply a delay if the clock has a reference point to avoid
     * messing up the timings if the stream was paused then seeked
     */
    const vlc_tick_t delay = now - main_clock->pause_date;
    if (main_clock->last.system != VLC_TICK_INVALID)
        main_clock->last.system += delay;
    if (main_clock->first_pcr.system != VLC_TICK_INVALID)
        main_clock->first_pcr.system += delay;
    if (main_clock->start_time.system != VLC_TICK_INVALID)
        main_clock->start_time.system += delay;
    if (main_clock->wait_sync_ref.system != VLC_TICK_INVALID)
        main_clock->wait_sync_ref.system += delay;
    main_clock->pause_date = VLC_TICK_INVALID;
    vlc_cond_broadcast(&main_clock->cond);
}

void vlc_clock_main_Delete(vlc_clock_main_t *main_clock)
{
    assert(main_clock->rc == 1);
    if (main_clock->logger != NULL)
        vlc_LogDestroy(main_clock->logger);

    assert(main_clock->listeners.size == 0);
    vlc_vector_destroy(&main_clock->listeners);

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

vlc_tick_t vlc_clock_Update(vlc_clock_t *clock, vlc_tick_t system_now,
                            vlc_tick_t ts, double rate)
{
    AssertLocked(clock);
    return clock->ops->update(clock, system_now, ts, rate, 0, 0);
}

vlc_tick_t vlc_clock_UpdateVideo(vlc_clock_t *clock, vlc_tick_t system_now,
                                 vlc_tick_t ts, double rate,
                                 unsigned frame_rate, unsigned frame_rate_base)
{
    AssertLocked(clock);
    return clock->ops->update(clock, system_now, ts, rate, frame_rate, frame_rate_base);
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

vlc_tick_t vlc_clock_ConvertToSystem(vlc_clock_t *clock,
                                     vlc_tick_t system_now, vlc_tick_t ts,
                                     double rate)
{
    AssertLocked(clock);
    return clock->ops->to_system(clock, system_now, ts, rate);
}

static const struct vlc_clock_ops master_ops = {
    .update = vlc_clock_master_update,
    .reset = vlc_clock_master_reset,
    .set_delay = vlc_clock_master_set_delay,
    .to_system = vlc_clock_master_to_system,
    .start = vlc_clock_output_start,
};

static const struct vlc_clock_ops slave_ops = {
    .update = vlc_clock_slave_update,
    .reset = vlc_clock_slave_reset,
    .set_delay = vlc_clock_slave_set_delay,
    .to_system = vlc_clock_slave_to_system,
    .start = vlc_clock_output_start,
};

static const struct vlc_clock_ops input_master_ops = {
    .update = vlc_clock_master_update,
    .reset = vlc_clock_master_reset,
    .set_delay = vlc_clock_master_set_delay,
    .to_system = vlc_clock_master_to_system,
    .start = vlc_clock_input_start,
};

static const struct vlc_clock_ops input_slave_ops = {
    .update = vlc_clock_slave_update,
    .reset = vlc_clock_slave_reset,
    .set_delay = vlc_clock_slave_set_delay,
    .to_system = vlc_clock_slave_to_system,
    .start = vlc_clock_input_start,
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
    assert(main_clock->offset == VLC_TICK_INVALID);

    /* Override the master ES clock if it exists */
    if (main_clock->master != NULL)
        main_clock->master->ops = &slave_ops;

    clock->ops = &input_master_ops;
    main_clock->input_master = clock;
    main_clock->rc++;

    return clock;
}

vlc_clock_t *vlc_clock_main_CreateInputSlave(vlc_clock_main_t *main_clock)
{
    vlc_mutex_assert(&main_clock->lock);

    /* The master has always the 0 priority */
    vlc_clock_t *clock = vlc_clock_main_Create(main_clock, "input", 0, NULL, NULL);
    if (!clock)
        return NULL;

    assert(main_clock->input_master == NULL);

    /* Even if the master ES clock has already been created, it should not
     * have updated any points */
    assert(main_clock->offset == VLC_TICK_INVALID);

    clock->ops = &input_slave_ops;
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

void vlc_clock_Start(vlc_clock_t *clock,
                     vlc_tick_t start_date,
                     vlc_tick_t first_ts)
{
    clock->ops->start(clock, start_date, first_ts);
}
