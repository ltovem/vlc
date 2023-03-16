// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * statistic.h : vout statistic
 *****************************************************************************
 * Copyright (C) 2009 Laurent Aimar
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *****************************************************************************/

#ifndef LIBVLC_VOUT_STATISTIC_H
# define LIBVLC_VOUT_STATISTIC_H
# include <stdatomic.h>

/* NOTE: Both statistics are atomic on their own, so one might be older than
 * the other one. Currently, only one of them is updated at a time, so this
 * is a non-issue. */
typedef struct {
    atomic_uint displayed;
    atomic_uint lost;
    atomic_uint late;
} vout_statistic_t;

static inline void vout_statistic_Init(vout_statistic_t *stat)
{
    atomic_init(&stat->displayed, 0);
    atomic_init(&stat->lost, 0);
    atomic_init(&stat->late, 0);
}

static inline void vout_statistic_Clean(vout_statistic_t *stat)
{
    (void) stat;
}

static inline void vout_statistic_GetReset(vout_statistic_t *stat,
                                           unsigned *restrict displayed,
                                           unsigned *restrict lost,
                                           unsigned *restrict late)
{
    *displayed = atomic_exchange_explicit(&stat->displayed, 0,
                                          memory_order_relaxed);
    *lost = atomic_exchange_explicit(&stat->lost, 0, memory_order_relaxed);
    *late = atomic_exchange_explicit(&stat->late, 0, memory_order_relaxed);
}

static inline void vout_statistic_AddDisplayed(vout_statistic_t *stat,
                                               int displayed)
{
    atomic_fetch_add_explicit(&stat->displayed, displayed,
                              memory_order_relaxed);
}

static inline void vout_statistic_AddLost(vout_statistic_t *stat, int lost)
{
    atomic_fetch_add_explicit(&stat->lost, lost, memory_order_relaxed);
}

static inline void vout_statistic_AddLate(vout_statistic_t *stat, int late)
{
    atomic_fetch_add_explicit(&stat->late, late, memory_order_relaxed);
}

#endif
