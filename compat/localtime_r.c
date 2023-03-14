/*****************************************************************************
 * localtime_r.c: POSIX localtime_r() replacement
 *****************************************************************************
 * Copyright © 1998-2015 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#if defined(__STDC_LIB_EXT1__) && (__STDC_LIB_EXT1__ >= 20112L)
# define __STDC_WANT_LIB_EXT1__ 1
#else
# define __STDC_WANT_LIB_EXT1__ 0
#endif
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <time.h>

/* If localtime_r() is not provided, we assume localtime() uses
 * thread-specific storage. */
struct tm *localtime_r (const time_t *timep, struct tm *result)
{
#if (__STDC_WANT_LIB_EXT1__)
    return localtime_s(timep, result);
#elif defined (_WIN32)
    errno_t ret = localtime_s(result, timep);
    if (ret == 0)
        return result;
    errno = ret;
    return NULL;
#else
# warning localtime_r() not implemented!
    return gmtime_r(timep, result);
#endif
}
