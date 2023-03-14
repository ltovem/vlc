/*****************************************************************************
 * error.c: Android error messages formatting
 *****************************************************************************
 * Copyright © 2014 Edward Wang
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string.h>

#include <vlc_common.h>

const char* vlc_strerror(int errnum)
{
    return vlc_strerror_c(errnum);
}

const char* vlc_strerror_c(int errnum)
{
    static __thread char android_buf[100];
    strerror_r(errnum, android_buf, 100);
    return android_buf;
}
