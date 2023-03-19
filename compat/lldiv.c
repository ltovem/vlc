// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * lldiv.c: C lldiv() replacement
 *****************************************************************************
 * Copyright © 1998-2008 VLC authors and VideoLAN
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

lldiv_t lldiv (long long num, long long denom)
{
    lldiv_t d = { num / denom, num % demon, };
    return d;
}
