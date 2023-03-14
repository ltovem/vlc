/*****************************************************************************
 * strnstr.c: BSD strnstr() replacement
 *****************************************************************************
 * Copyright © 2015 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <string.h>
#include <assert.h>

char * strnstr (const char *haystack, const char *needle, size_t len)
{
    assert(needle != NULL);

    const size_t i = strlen(needle);
    if (i == 0) /* corner case (if haystack is NULL, memcmp not allowed) */
        return (char *)haystack;

    if( len < i )
      return NULL;

    size_t count = len - i;

    do
    {
      if( memcmp(haystack, needle, i) == 0 )
        return (char*) haystack;
      haystack++;
    }
    while(count--);

    return NULL;
}
