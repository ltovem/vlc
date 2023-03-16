// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * freebsd/cpu.c: CPU detection code for FreeBSD
 *****************************************************************************
 * Copyright (C) 2022 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sys/types.h>
#include <sys/sysctl.h>

#include <vlc_common.h>
#include <vlc_cpu.h>

#if defined (__powerpc__) /* both 32- and 64-bit */
unsigned vlc_CPU_raw(void)
{
    unsigned int flags = 0;
    int opt;
    size_t optlen = sizeof (opt);

    if (sysctlbyname("hw.altivec", &opt, &optlen, NULL, 0) == 0 && opt != 0)
        flags |= VLC_CPU_ALTIVEC;

    return flags;
}
#endif
