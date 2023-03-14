/*****************************************************************************
 * i18n_atof.c: Test for vlc_atof_c()
 *****************************************************************************
 * Copyright (C) 2006 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include "vlc_charset.h"

#undef NDEBUG
#include <assert.h>

int main (void)
{
    const char dot9[] = "999999.999999";
    const char comma9[] = "999999,999999";
    const char sharp9[] = "999999#999999";
    char *end;

    assert(vlc_atof_c("0") == 0.);
    assert(vlc_atof_c("1") == 1.);
    assert(vlc_atof_c("1.") == 1.);
    assert(vlc_atof_c("1,") == 1.);
    assert(vlc_atof_c("1#") == 1.);
    assert(vlc_atof_c(dot9) == 999999.999999);
    assert(vlc_atof_c(comma9) == 999999.);
    assert(vlc_atof_c(sharp9) == 999999.);
    assert(vlc_atof_c("invalid") == 0.);

    assert((vlc_strtod_c(dot9, &end ) == 999999.999999) && (*end == '\0'));
    assert((vlc_strtod_c(comma9, &end ) == 999999.) && (*end == ','));
    assert((vlc_strtod_c(sharp9, &end ) == 999999.) && (*end == '#'));

    return 0;
}
