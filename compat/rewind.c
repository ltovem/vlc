/*****************************************************************************
 * rewind.c: C rewind replacement
 *****************************************************************************
 * Copyright © 1998-2008 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>

void rewind (FILE *stream)
{
    fseek (stream, 0L, SEEK_SET);
    clearerr (stream);
}
