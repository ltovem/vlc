// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * clock_nanosleep.c: POSIX clock_nanosleep() replacement
 *****************************************************************************
 * Copyright © 2020 VLC authors and VideoLAN
 *
 * Author: Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#ifndef _WIN32

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <assert.h>
#include <pthread.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/sysctl.h>

# ifdef __APPLE__
#include <mach/clock_types.h>
# endif

int clock_nanosleep(clockid_t clock_id, int flags,
        const struct timespec *rqtp, struct timespec *rmtp)
{
    // Validate timespec
    if (rqtp == NULL || rqtp->tv_sec < 0 ||
            rqtp->tv_nsec < 0 || (unsigned long)rqtp->tv_nsec >= 1000*1000*1000) {
        errno = EINVAL;
        return -1;
    }

    // Validate clock
    switch (clock_id) {
        case CLOCK_MONOTONIC:
        case CLOCK_REALTIME:
            break;
        default:
            errno = EINVAL;
            return -1;
    }

    if (flags == TIMER_ABSTIME) {
        struct timespec ts_rel;
        struct timespec ts_now;

        do {
            // Get current time with requested clock
            if (clock_gettime(clock_id, &ts_now) != 0)
                return -1;

            // Calculate relative timespec
            ts_rel.tv_sec  = rqtp->tv_sec  - ts_now.tv_sec;
            ts_rel.tv_nsec = rqtp->tv_nsec - ts_now.tv_nsec;
            if (ts_rel.tv_nsec < 0) {
                ts_rel.tv_sec  -= 1;
                ts_rel.tv_nsec += 1000*1000*1000;
            }

            // Check if time already elapsed
            if (ts_rel.tv_sec < 0 || (ts_rel.tv_sec == 0 && ts_rel.tv_nsec == 0)) {
                pthread_testcancel();
                return 0;
            }

            // "The absolute clock_nanosleep() function has no effect on the
            // structure referenced by rmtp", so do not pass rmtp here
        } while (nanosleep(&ts_rel, NULL) == 0);

        // If nanosleep failed or was interrupted by a signal,
        // return so the caller can handle it appropriately
        return -1;
    } else if (flags == 0) {
        return nanosleep(rqtp, rmtp);
    } else {
        // Invalid flags
        errno = EINVAL;
        return -1;
    }
}

#endif
