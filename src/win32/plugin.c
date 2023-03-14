/*****************************************************************************
 * plugin.c : Low-level dynamic library handling
 *****************************************************************************
 * Copyright (C) 2001-2011 VLC authors and VideoLAN
 *
 * Authors: Sam Hocevar <sam@zoy.org>
 *          Ethan C. Baldridge <BaldridgeE@cadmus.com>
 *          Hans-Peter Jansen <hpj@urpla.net>
 *          Gildas Bazin <gbazin@videolan.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_charset.h>
#include "modules/modules.h"
#include <windows.h>
#include <wchar.h>

char *vlc_dlerror(void)
{
    wchar_t wmsg[256];
    int i = 0, i_error = GetLastError();

    FormatMessageW( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, i_error, MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
                    wmsg, 256, NULL );

    /* Go to the end of the string */
    while( !wmemchr( L"\r\n\0", wmsg[i], 3 ) )
        i++;

    snwprintf( wmsg + i, 256 - i, L" (error %i)", i_error );
    return FromWide( wmsg );
}

void *vlc_dlopen(const char *psz_file, bool lazy)
{
    wchar_t *wfile = ToWide (psz_file);
    if (wfile == NULL)
        return NULL;

    HMODULE handle = NULL;
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
    DWORD mode;
    if (SetThreadErrorMode (SEM_FAILCRITICALERRORS, &mode) != 0)
    {
        handle = LoadLibraryExW(wfile, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        SetThreadErrorMode (mode, NULL);
    }
#elif WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
    handle = LoadPackagedLibrary( wfile, 0 );
#elif defined(WINAPI_FAMILY_GAMES) && (WINAPI_FAMILY == WINAPI_FAMILY_GAMES)
    // SEM_FAILCRITICALERRORS is not available on Xbox, error messages are not
    // possible. This is a sandboxed environment where the library dependencies
    // need to be managed manually per platform.
    handle = LoadLibraryExW(wfile, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
#else
#error Unknown Windows Platform!
#endif
    free (wfile);

    (void) lazy;
    return handle;
}

int vlc_dlclose(void *handle)
{
    FreeLibrary( handle );
    return 0;
}

void *vlc_dlsym(void *handle, const char *psz_function)
{
    return (void *)GetProcAddress(handle, psz_function);
}
