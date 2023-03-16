// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * clock_internal.h: Clock internal functions
 *****************************************************************************
 * Copyright (C) 2018 VLC authors and VideoLAN
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *          Laurent Aimar < fenrir _AT_ videolan _DOT_ org >
 *****************************************************************************/

#ifndef CLOCK_INTERNAL_H
# define CLOCK_INTERNAL_H

#include <vlc_common.h>

/* Maximum gap allowed between two CRs. */
#define CR_MAX_GAP VLC_TICK_FROM_SEC(60)

/*****************************************************************************
 * Structures
 *****************************************************************************/

/**
 * This structure holds long term moving average
 */
typedef struct
{
    double value; /* The average value */
    int count; /* The number of sample evaluated */
    int range; /* The maximum range of sample on which we calculate the average*/
} average_t;

void AvgInit(average_t *, int range);
void AvgClean(average_t *);

void AvgReset(average_t *);
void AvgResetAndFill(average_t *, double value);

/*  calculates (previous_average * (range - 1) + new_value)/range */
void AvgUpdate(average_t *, double value);

double AvgGet(average_t *);
void AvgRescale(average_t *, int range);

/* */
typedef struct
{
    vlc_tick_t system;
    vlc_tick_t stream;
} clock_point_t;

static inline clock_point_t clock_point_Create(vlc_tick_t system, vlc_tick_t stream)
{
    return (clock_point_t) { .system = system, .stream = stream };
}

#endif
