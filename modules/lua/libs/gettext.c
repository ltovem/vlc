// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * gettext.c
 *****************************************************************************
 * Copyright (C) 2009 the VideoLAN team
 *
 * Authors: Antoine Cellerier <dionoea at videolan tod org>
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

#include "../vlc.h"
#include "../libs.h"

/*****************************************************************************
 * Libvlc gettext support
 *****************************************************************************/
static int vlclua_gettext( lua_State *L )
{
    lua_pushstring( L, _( luaL_checkstring( L, 1 ) ) );
    return 1;
}

static int vlclua_gettext_noop( lua_State *L )
{
    lua_settop( L, 1 );
    return 1;
}

/*****************************************************************************
 *
 *****************************************************************************/
static const luaL_Reg vlclua_gettext_reg[] = {
    { "_", vlclua_gettext },
    { "N_", vlclua_gettext_noop },

    { NULL, NULL }
};

void luaopen_gettext( lua_State *L )
{
    lua_newtable( L );
    luaL_register( L, NULL, vlclua_gettext_reg );
    lua_setfield( L, -2, "gettext" );
}
