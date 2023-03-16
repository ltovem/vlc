// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * plugin.c : Low-level dynamic library handling
 *****************************************************************************
 * Copyright (C) 2001-2007 VLC authors and VideoLAN
 * Copyright (C) 2012 KO Myung-Hun
 *
 * Authors: Sam Hocevar <sam@zoy.org>
 *          Ethan C. Baldridge <BaldridgeE@cadmus.com>
 *          Hans-Peter Jansen <hpj@urpla.net>
 *          Gildas Bazin <gbazin@videolan.org>
 *          KO Myung-Hun <komh@chollian.net>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string.h>
#include <sys/types.h>
#include <dlfcn.h>

#include <vlc_common.h>
#include <vlc_charset.h>
#include "modules/modules.h"

void *vlc_dlopen(const char *psz_file, bool lazy )
{
    const int flags = lazy ? RTLD_LAZY : RTLD_NOW;
    char *path = ToLocaleDup( psz_file );
    if (unlikely(path == NULL))
        return NULL;

    void *handle = dlopen( path, flags );
    free( path );
    return handle;
}

int vlc_dlclose(void *handle)
{
    return dlclose( handle );
}

void *vlc_dlsym(void *handle, const char *psz_function)
{
    char buf[strlen(psz_function) + 2];
    buf[0] = '_';
    strcpy(buf + 1, psz_function);
    return dlsym( handle, buf );
}

char *vlc_dlerror(void)
{
    const char *errmsg = dlerror();
    return (errmsg != NULL) ? FromLocaleDup(errmsg) : NULL;
}
