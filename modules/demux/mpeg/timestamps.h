/*****************************************************************************
 * timestamps.h: MPEG TS/PS Timestamps helpers
 *****************************************************************************
 * Copyright (C) 2004-2016 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef VLC_MPEG_TIMESTAMPS_H
#define VLC_MPEG_TIMESTAMPS_H

#define FROM_SCALE_NZ(x) ((vlc_tick_t)((x) * 100 / 9))
#define TO_SCALE_NZ(x)   ((x) * 9 / 100)

#define FROM_SCALE(x) (VLC_TICK_0 + FROM_SCALE_NZ(x))
#define TO_SCALE(x)   TO_SCALE_NZ((x) - VLC_TICK_0)

typedef int64_t stime_t;

static inline stime_t TimeStampWrapAround( stime_t i_first_pcr, stime_t i_time )
{
    stime_t i_adjust = 0;
    if( i_first_pcr > 0x0FFFFFFFF && i_time < 0x0FFFFFFFF )
        i_adjust = 0x1FFFFFFFF;

    return i_time + i_adjust;
}

#endif
