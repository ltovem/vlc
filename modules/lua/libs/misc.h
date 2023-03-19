// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * misc.h
 *****************************************************************************
 * Copyright (C) 2007-2008 the VideoLAN team
 *
 * Authors: Antoine Cellerier <dionoea at videolan tod org>
 *          Pierre d'Herbemont <pdherbemont # videolan.org>
 *****************************************************************************/

#ifndef VLC_LUA_MISC_H
#define VLC_LUA_MISC_H

void vlclua_set_object( lua_State *, void *id, void *value );
void *vlclua_get_object( lua_State *, void *id );

vlc_object_t * vlclua_get_this( lua_State * );

int vlclua_push_ret( lua_State *, int );
vlc_player_t *vlclua_get_player_internal(lua_State *L);

#endif

