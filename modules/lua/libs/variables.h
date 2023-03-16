// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * variables.h: Generic lua<->vlc variables interface
 *****************************************************************************
 * Copyright (C) 2007-2008 the VideoLAN team
 *
 * Authors: Antoine Cellerier <dionoea at videolan tod org>
 *          Pierre d'Herbemont <pdherbemont # videolan.org>
 *****************************************************************************/

#ifndef VLC_LUA_VARIABLES_H
#define VLC_LUA_VARIABLES_H

int vlclua_var_toggle_or_set( lua_State *, vlc_object_t *, const char * );

#define vlclua_var_toggle_or_set( a, b, c ) \
        vlclua_var_toggle_or_set( a, VLC_OBJECT( b ), c )

#endif

