// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * error.c: Darwin error messages handling
 *****************************************************************************
 * Copyright © 2006-2013 Rémi Denis-Courmont
 *           © 2013 Felix Paul Kühne
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <errno.h>

#include <vlc_common.h>

const char *vlc_strerror_c(int errnum)
{
    /* We cannot simply use strerror() here, since it is not thread-safe. */
    if ((unsigned)errnum < (unsigned)sys_nerr)
        return sys_errlist[errnum];

    return _("Unknown error");
}

const char *vlc_strerror(int errnum)
{
    return vlc_strerror_c(errnum);
}
