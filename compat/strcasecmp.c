/*****************************************************************************
 * strcasecmp.c: POSIX strcasecmp() replacement
 *****************************************************************************
 * Copyright © 1998-2008 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <string.h>
#include <ctype.h>
#include <assert.h>

int strcasecmp (const char *s1, const char *s2)
{
#ifdef _WIN32
    return _stricmp (s1, s2);
#else
    for (size_t i = 0;; i++)
    {
        unsigned char c1 = s1[i], c2 = s2[i];
        int d = tolower (c1) - tolower (c2);
        if (d || !c1)
            return d;
        assert (c2);
    }
#endif
}
