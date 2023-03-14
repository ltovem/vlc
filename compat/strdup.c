/*****************************************************************************
 * strdup.c: POSIX strdup() replacement
 *****************************************************************************
 * Copyright © 1998-2008 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <string.h>
#include <stdlib.h>

char *strdup (const char *str)
{
    size_t len = strlen (str) + 1;
    char *res = malloc (len);
    if (res)
        memcpy (res, str, len);
    return res;
}
