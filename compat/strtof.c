// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * strtof.c: C strtof() replacement
 *****************************************************************************
 * Copyright © 1998-2009 VLC authors and VideoLAN
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

#ifndef __ANDROID__
float strtof (const char *str, char **end)
{
    return strtod (str, end);
}
#endif
