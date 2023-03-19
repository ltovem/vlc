// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * vasprintf.c: GNU vasprintf() replacement
 *****************************************************************************
 * Copyright © 1998-2009 VLC authors and VideoLAN
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

int vasprintf (char **strp, const char *fmt, va_list ap)
{
    va_list args;
    int len;

    va_copy (args, ap);
    len = vsnprintf (NULL, 0, fmt, args);
    va_end (args);

    char *str = malloc (len + 1);
    if (str != NULL)
    {
        int len2;

        va_copy (args, ap);
        len2 = vsprintf (str, fmt, args);
        assert (len2 == len);
        va_end (args);
    }
    else
    {
        len = -1;
#ifndef NDEBUG
        str = (void *)(intptr_t)0x41414141; /* poison */
#endif
    }
    *strp = str;
    return len;
}
