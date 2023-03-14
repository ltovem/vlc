/*****************************************************************************
 * asprintf.c: GNU asprintf() replacement
 *****************************************************************************
 * Copyright © 1998-2009 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdarg.h>

int asprintf (char **strp, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start (ap, fmt);
    ret = vasprintf (strp, fmt, ap);
    va_end (ap);
    return ret;
}
