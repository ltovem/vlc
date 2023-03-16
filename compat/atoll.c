// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * atoll.c: C atoll() replacement
 *****************************************************************************
 * Copyright © 1998-2009 VLC authors and VideoLAN
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

long long atoll (const char *str)
{
    return strtoll (str, NULL, 10);
}
