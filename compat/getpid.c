/*****************************************************************************
 * getpid.c: POSIX getpid() replacement
 *****************************************************************************
 * Copyright © 2009 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <sys/types.h>
#ifdef _WIN32
# include <windows.h>
#endif

pid_t getpid (void)
{
#if defined (_WIN32)
    return (pid_t) GetCurrentProcessId ();
#else
# error Unimplemented!
#endif
}
