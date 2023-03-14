/*****************************************************************************
 * version.c: LibVLC version infos
 *****************************************************************************
 * Copyright (C) 1998-2008 VLC authors and VideoLAN
 *
 * Authors: Vincent Seguin <seguin@via.ecp.fr>
 *          Samuel Hocevar <sam@zoy.org>
 *          Gildas Bazin <gbazin@videolan.org>
 *          Derk-Jan Hartman <hartman at videolan dot org>
 *          Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>

/*****************************************************************************
 * VLC_CompileBy, VLC_CompileHost
 * VLC_Compiler, VLC_Changeset
 *****************************************************************************/
#define DECLARE_VLC_VERSION( func, var )                                    \
const char * VLC_##func ( void )                                            \
{                                                                           \
    return VLC_##var ;                                                      \
}

DECLARE_VLC_VERSION( CompileBy, COMPILE_BY )
DECLARE_VLC_VERSION( CompileHost, COMPILE_HOST )
DECLARE_VLC_VERSION( Compiler, COMPILER )
