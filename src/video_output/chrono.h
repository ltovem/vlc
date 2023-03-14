/*****************************************************************************
 * chrono.h: vout chrono
 *****************************************************************************
 * Copyright (C) 2009-2010 Laurent Aimar
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef LIBVLC_VOUT_CHRONO_H
#define LIBVLC_VOUT_CHRONO_H

#include <assert.h>

typedef struct {
    int     shift;
    vlc_tick_t avg;
    unsigned avg_count;

    int     shift_mad;
    vlc_tick_t mad; /* mean absolute deviation */
    unsigned mad_count;

    vlc_tick_t start;
} vout_chrono_t;

static inline void vout_chrono_Init(vout_chrono_t *chrono, int shift, vlc_tick_t avg_initial)
{
    chrono->avg_count   = 0;
    chrono->mad_count   = 0;

    chrono->shift       = shift;
    chrono->avg         = avg_initial;

    chrono->shift_mad   = shift+1;
    chrono->mad         = 0;

    chrono->start = VLC_TICK_INVALID;
}

static inline void vout_chrono_Start(vout_chrono_t *chrono)
{
    chrono->start = vlc_tick_now();
}

static inline vlc_tick_t vout_chrono_GetHigh(vout_chrono_t *chrono)
{
    return chrono->avg + 2 * chrono->mad;
}

static inline vlc_tick_t vout_chrono_GetLow(vout_chrono_t *chrono)
{
    return __MAX(chrono->avg - 2 * chrono->mad, 0);
}

static inline void vout_chrono_Stop(vout_chrono_t *chrono)
{
    assert(chrono->start != VLC_TICK_INVALID);

    const vlc_tick_t duration = vlc_tick_now() - chrono->start;

    if (chrono->avg_count == 0)
    {
        /* Overwrite the arbitrary initial values with the real first sample */
        chrono->avg = duration;
        chrono->avg_count = 1;
    }
    else
    {
        /* Update average only if the current point is 'valid' */
        if( duration < vout_chrono_GetHigh( chrono ) )
        {
            if (chrono->avg_count < (1u << chrono->shift))
                ++chrono->avg_count;
            chrono->avg = ((chrono->avg_count - 1) * chrono->avg + duration) / chrono->avg_count;
        }

        const vlc_tick_t abs_diff = llabs( duration - chrono->avg );

        /* Always update the mean absolute deviation */
        if (chrono->mad_count < (1u << chrono->shift_mad))
            ++chrono->mad_count;
        chrono->mad = ((chrono->mad_count - 1) * chrono->mad + abs_diff) / chrono->mad_count;
    }

    /* For assert */
    chrono->start = VLC_TICK_INVALID;
}

#endif
