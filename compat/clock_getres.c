// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * clock_getres.c: POSIX clock_getres() replacement
 *****************************************************************************
 * Copyright © 2020 VLC authors and VideoLAN
 *
 * Author: Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#ifdef __APPLE__

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <sys/errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <mach/clock_types.h>

int clock_getres(clockid_t clock_id, struct timespec *tp)
{
    switch (clock_id) {
        case CLOCK_MONOTONIC:
        case CLOCK_REALTIME:
            // For realtime, it is using gettimeofday and for
            // the monotonic time it is relative to the system
            // boot time. Both of these use timeval, which has
            // at best microsecond precision.
            tp->tv_sec = 0;
            tp->tv_nsec = NSEC_PER_USEC;
            break;
        default:
            errno = EINVAL;
            return -1;
    }

    return 0;
}

#endif
