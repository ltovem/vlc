// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * timespec_get.c: C11 timespec_get() replacement
 *****************************************************************************
 * Copyright © 2015 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef _WIN32
#include <windows.h>

int timespec_get(struct timespec *ts, int base)
{
    FILETIME ft;
    ULARGE_INTEGER s;
    ULONGLONG t;

    if (base != TIME_UTC)
        return 0;

    GetSystemTimeAsFileTime(&ft);
    s.LowPart = ft.dwLowDateTime;
    s.HighPart = ft.dwHighDateTime;
    t = s.QuadPart - 116444736000000000ULL;
    ts->tv_sec = t / 10000000;
    ts->tv_nsec = ((int) (t % 10000000)) * 100;
    return base;
}
#else /* !_WIN32 */

#include <time.h>
#include <unistd.h> /* _POSIX_TIMERS */
#ifndef _POSIX_TIMERS
#define _POSIX_TIMERS (-1)
#endif
#if (_POSIX_TIMERS <= 0)
# include <sys/time.h> /* gettimeofday() */
#endif

int timespec_get(struct timespec *ts, int base)
{
    switch (base)
    {
        case TIME_UTC:
#if (_POSIX_TIMERS >= 0)
            if (clock_gettime(CLOCK_REALTIME, ts) == 0)
                break;
#endif
#if (_POSIX_TIMERS <= 0)
        {
            struct timeval tv;

            if (gettimeofday(&tv, NULL) == 0)
            {
                ts->tv_sec = tv.tv_sec;
                ts->tv_nsec = tv.tv_usec * 1000;
                break;
            }
        }
#endif
            /* fall through */
        default:
            return 0;
    }
    return base;
}
#endif /* !_WIN32 */
