// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * atof.c: C atof() replacement
 *****************************************************************************
 * Copyright © 1998-2009 VLC authors and VideoLAN
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

#ifndef __ANDROID__
double atof (const char *str)
{
    return strtod (str, NULL);
}
#endif
