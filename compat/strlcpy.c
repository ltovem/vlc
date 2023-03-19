// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * strlcpy.c: BSD strlcpy() replacement
 *****************************************************************************
 * Copyright © 2006, 2009 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <string.h>

/**
 * Copy a string to a sized buffer. The result is always nul-terminated
 * (contrary to strncpy()).
 *
 * @param dest destination buffer
 * @param src string to be copied
 * @param len maximum number of characters to be copied plus one for the
 * terminating nul.
 *
 * @return strlen(src)
 */
size_t strlcpy (char *tgt, const char *src, size_t bufsize)
{
    size_t length = strlen(src);

    if (bufsize > length)
        memcpy(tgt, src, length + 1);
    else
    if (bufsize > 0)
        memcpy(tgt, src, bufsize - 1), tgt[bufsize - 1] = '\0';

    return length;
}
