/*****************************************************************************
 * service_discoverer.c
 *****************************************************************************
 * Copyright (C) 2020 the VideoLAN team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_player.h>
#include <vlc_input_item.h>
#include <vlc_vector.h>
#include <vlc_services_discovery.h>

#include "../vlc.h"
#include "../libs.h"

#include "misc.h"

struct sd_items_vec VLC_VECTOR(input_item_t*);

typedef struct vlclua_sd_sys_t
{
    struct services_discovery_owner_t owner;
    services_discovery_t* sd;
    struct sd_items_vec items;
    vlc_mutex_t mutex;
} vlclua_sd_sys_t;

static bool vlclua_service_compare( input_item_t* rhs,
                                    input_item_t* lhs )
{
    return !strcmp( rhs->psz_name, lhs->psz_name ) &&
           !(rhs->i_type == lhs->i_type);
}

static int vlclua_sd_list( lua_State* L )
{
    vlclua_sd_sys_t* sys = (vlclua_sd_sys_t*)luaL_checkudata( L, 1, "service_discoverer" );

    input_item_t *activeService;
    vlc_player_t* player = vlclua_get_player_internal( L );
    vlc_player_Lock( player );
    activeService = vlc_player_GetCurrentMedia( player );
    if ( activeService )
        input_item_Hold( activeService );
    vlc_player_Unlock( player );

    lua_createtable( L, sys->items.size, 0 );
    vlc_mutex_lock( &sys->mutex );
    for ( size_t i = 0; i < sys->items.size; ++i )
    {
        lua_newtable( L );

        lua_pushinteger( L, sys->items.data[i]->i_mediaID );
        lua_setfield( L, -2, "id" );
        lua_pushstring( L, sys->items.data[i]->psz_name );
        lua_setfield( L, -2, "name" );
        lua_pushinteger( L, sys->items.data[i]->i_type );
        lua_setfield( L, -2, "type" );

        bool selected = activeService != NULL &&
                vlclua_service_compare( activeService, sys->items.data[i] );
        lua_pushboolean( L, selected );
        lua_setfield( L, -2, "selected" );

        lua_rawseti( L, -2, i + 1 );
    }
    vlc_mutex_unlock( &sys->mutex );

    if ( activeService )
        input_item_Release( activeService );

    return 1;
}

static const luaL_Reg vlclua_sd_obj_reg[] = {
    { "list", vlclua_sd_list },
    { NULL, NULL }
};

static int vlclua_sd_obj_delete( lua_State *L )
{
    vlclua_sd_sys_t* sys = (vlclua_sd_sys_t*)luaL_checkudata( L, 1, "service_discoverer" );
    for ( size_t i = 0; i < sys->items.size; ++i )
        input_item_Release( sys->items.data[i] );
    vlc_vector_destroy( &sys->items );
    return 0;
}

static void vlclua_sd_on_item_added( struct services_discovery_t* sd,
                                        struct input_item_t* parent,
                                        struct input_item_t* item,
                                        const char *psz_cat )
{
    if( parent != NULL )
    {
        /* Do something */
    }
    else
        /* Do something */
    if( psz_cat )
    {
        /* Do something */
    }

    vlclua_sd_sys_t* sys = sd->owner.sys;
    vlc_mutex_lock( &sys->mutex );
    input_item_t* i = input_item_Hold( item );
    vlc_vector_push( &sys->items, i );
    vlc_mutex_unlock( &sys->mutex );
}

static void vlclua_sd_on_item_removed( struct services_discovery_t* sd,
                                        struct input_item_t* item )
{
    vlclua_sd_sys_t* sys = sd->owner.sys;

    vlc_mutex_lock( &sys->mutex );
    for ( size_t i = 0; i < sys->items.size; ++i )
    {
        if ( !vlclua_service_compare( item, sys->items.data[i] ) )
            continue;
        vlc_vector_remove( &sys->items, i );
        break;
    }
    vlc_mutex_unlock( &sys->mutex );
}

static int vlclua_sd_create( lua_State* L )
{
    vlclua_sd_sys_t* sys = lua_newuserdata( L, sizeof( *sys ) );
    if ( !sys )
        return 0;
    sys->owner.sys = sys;

    static const struct services_discovery_callbacks cbs = {
        .item_added = vlclua_sd_on_item_added,
        .item_removed = vlclua_sd_on_item_removed,
    };
    sys->owner.cbs = &cbs;
    vlc_vector_init( &sys->items );
    vlc_mutex_init( &sys->mutex );

    vlc_object_t *this = vlclua_get_this( L );
    const char* name = luaL_checkstring( L, 1 );

    sys->sd = vlc_sd_Create( this, name, &sys->owner );
    if ( !sys->sd )
    {
        vlc_vector_destroy( &sys->items );
        return 0;
    }

    if ( luaL_newmetatable( L, "service_discoverer" ) )
    {
        lua_newtable( L );
        luaL_register( L, NULL, vlclua_sd_obj_reg );
        lua_setfield( L, -2, "__index" );
        lua_pushcfunction( L, vlclua_sd_obj_delete );
        lua_setfield( L, -2, "__gc" );
    }
    lua_setmetatable( L, -2 );

    return 1;
}

static const luaL_Reg vlclua_sd_reg[] = {
    { "create", vlclua_sd_create },
    { NULL, NULL }
};

void luaopen_sd( lua_State *L )
{
    lua_newtable( L );
    luaL_register( L, NULL, vlclua_sd_reg );
    lua_setfield( L, -2, "sd" );
}
