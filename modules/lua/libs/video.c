/*****************************************************************************
 * video.c: Generic lua interface functions
 *****************************************************************************
 * Copyright (C) 2007-2008 the VideoLAN team
 *
 * Authors: Antoine Cellerier <dionoea at videolan tod org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#ifndef  _GNU_SOURCE
#   define  _GNU_SOURCE
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_vout.h>

#include "../vlc.h"
#include "../libs.h"
#include "input.h"
#include "variables.h"

/*****************************************************************************
 * Vout control
 *****************************************************************************/
static int vlclua_fullscreen( lua_State *L )
{
    int i_ret;

    vout_thread_t *p_vout = vlclua_get_vout_internal(L);
    if( !p_vout )
        return vlclua_error( L );

    i_ret = vlclua_var_toggle_or_set( L, p_vout, "fullscreen" );

    vout_Release(p_vout);
    return i_ret;
}

/*****************************************************************************
 *
 *****************************************************************************/
static const luaL_Reg vlclua_video_reg[] = {
    { "fullscreen", vlclua_fullscreen },
    { NULL, NULL }
};

void luaopen_video( lua_State *L )
{
    lua_newtable( L );
    luaL_register( L, NULL, vlclua_video_reg );
    lua_setfield( L, -2, "video" );
}
