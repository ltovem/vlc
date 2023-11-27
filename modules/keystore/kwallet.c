/*****************************************************************************
 * kwallet.c: KWallet keystore module
 *****************************************************************************
 * Copyright © 2015-2016 VLC authors, VideoLAN and VideoLabs
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_keystore.h>
#include <vlc_url.h>
#include <vlc_plugin.h>
#include <vlc_strings.h>
#include <vlc_interrupt.h>
#include <vlc_memstream.h>
#include <vlc_vector.h>

#include SDBUS_HEADER

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <poll.h>
#include <errno.h>
#include <assert.h>

typedef struct VLC_VECTOR(vlc_keystore_entry) keystore_vector_t;
static int Open( vlc_object_t * );
static void Close( vlc_object_t * );

vlc_module_begin()
    set_shortname( N_("KWallet keystore") )
    set_description( N_("Secrets are stored via KWallet") )
    set_subcategory( SUBCAT_ADVANCED_MISC )
    set_capability( "keystore", 100 )
    set_callbacks( Open, Close )
vlc_module_end()

/* kwallet is the kde keyring. *
 * There are several entry categories, *
 * but we only use the "Password" category. *
 * It is juste a simple Entry name ( or key ) *
 * associated with a secret. *
 * Keys are urls formatted with : *
 *     _ Protocol *
 *     _ User ( optional ) *
 *     _ Server *
 *     _ Port ( optional ) *
 *     _ Path ( optional ) *
 *     _ Realm ( binary encrypted ) ( optional ) *
 *     _ Authtype ( binary encrypted ) ( optional ) *
 * Secrets are binary encrypted strings */

static const char* psz_folder = VLC_KEYSTORE_NAME;
static const char* psz_kwallet_interface = "org.kde.KWallet";

#define DBUS_INSTANCE_PREFIX "instance"
#define KWALLET_APP_ID "org.videolan.kwallet"

#define KWALLET_SERVICE "org.kde.kwalletd5"

#define KWALLET_PATH "/modules/kwalletd5"

typedef struct vlc_keystore_sys
{
    sd_bus* bus;
    int i_sid; /* service ID */
    int i_handle;
    char* psz_app_id;
    char* psz_wallet;
}  vlc_keystore_sys;

/* takes all values in the values of vlc_keystore_entry *
 * and formats them in a url key */
static char*
values2key( const char* const* ppsz_values, int b_search )
{
    char* psz_b64_realm = NULL;
    char* psz_b64_auth = NULL;
    int b_state = 0;

    if ( ( !ppsz_values[KEY_PROTOCOL] || !ppsz_values[KEY_SERVER] )
         && !b_search )
        return NULL;

    struct vlc_memstream ms;
    if ( vlc_memstream_open( &ms ) )
        return NULL;

    /* Protocol section */
    if ( ppsz_values[KEY_PROTOCOL] )
        vlc_memstream_printf( &ms, "%s://", ppsz_values[KEY_PROTOCOL] );
    else if ( b_search )
        vlc_memstream_printf( &ms, "*://" );

    /* User section */
    if ( ppsz_values[KEY_USER] )
        vlc_memstream_printf( &ms, "%s@", ppsz_values[KEY_USER] );
    else if ( b_search )
        vlc_memstream_printf( &ms, "*" );

    /* Server section */
    if ( ppsz_values[KEY_SERVER] )
        vlc_memstream_printf( &ms, "%s", ppsz_values[KEY_SERVER] );
    else if ( b_search )
        vlc_memstream_printf( &ms, "*" );

    /* Port section */
    if ( ppsz_values[KEY_PORT] )
        vlc_memstream_printf( &ms, ":%s", ppsz_values[KEY_PORT] );
    else if ( b_search )
        vlc_memstream_printf( &ms, "*" );

    /* Path section */
    if( ppsz_values[KEY_PATH] )
    {
        if( ppsz_values[KEY_PATH][0] != '/' )
            vlc_memstream_putc( &ms, '/' );

        vlc_memstream_puts( &ms, ppsz_values[KEY_PATH] );
    }
    else if ( b_search )
        vlc_memstream_printf( &ms, "*" );

    /* Realm and authtype section */
    if ( ppsz_values[KEY_REALM] || ppsz_values[KEY_AUTHTYPE] )
    {
        vlc_memstream_printf( &ms, "\?" );
        /* Realm section */
        if ( ppsz_values[KEY_REALM] || b_search )
        {
            if ( ppsz_values[KEY_REALM] )
            {
                psz_b64_realm = vlc_b64_encode_binary( ( uint8_t* )ppsz_values[KEY_REALM],
                                                       strlen(ppsz_values[KEY_REALM] ) );
                if ( !psz_b64_realm )
                    goto end;
                vlc_memstream_printf( &ms, "realm=%s", psz_b64_realm );
            }
            else
                vlc_memstream_printf( &ms, "*" );

            if ( ppsz_values[KEY_AUTHTYPE] )
                vlc_memstream_printf( &ms, "&" );
        }

        /* Authtype section */
        if ( ppsz_values[KEY_AUTHTYPE] || b_search )
        {

            if ( ppsz_values[KEY_AUTHTYPE] )
            {
                psz_b64_auth = vlc_b64_encode_binary( ( uint8_t* )ppsz_values[KEY_AUTHTYPE],
                                                      strlen(ppsz_values[KEY_AUTHTYPE] ) );
                if ( !psz_b64_auth )
                    goto end;
                vlc_memstream_printf( &ms, "authtype=%s", psz_b64_auth );
            }
            else
                vlc_memstream_printf( &ms, "*" );
        }

    }
    else if ( b_search )
        vlc_memstream_printf( &ms, "*" );

    b_state = true;

end:
    free( psz_b64_realm );
    free( psz_b64_auth );
    char *psz_key = vlc_memstream_close( &ms ) == 0 ? ms.ptr : NULL;
    if ( !b_state )
    {
        free( psz_key );
        psz_key = NULL;
    }
    return psz_key;
}

/* Take an url key and splits it into vlc_keystore_entry values */
static int
key2values( char* psz_key, vlc_keystore_entry* p_entry )
{
    vlc_url_t url;
    int i_ret = VLC_ENOMEM;

    for ( int inc = 0 ; inc < KEY_MAX ; ++inc )
        p_entry->ppsz_values[inc] = NULL;

    vlc_UrlParse( &url, psz_key );

    if ( url.psz_protocol && !( p_entry->ppsz_values[KEY_PROTOCOL] =
                                strdup( url.psz_protocol ) ) )
        goto end;
    if ( url.psz_username && !( p_entry->ppsz_values[KEY_USER] =
                                strdup( url.psz_username ) ) )
        goto end;
    if ( url.psz_host && !( p_entry->ppsz_values[KEY_SERVER] =
                            strdup( url.psz_host ) ) )
        goto end;
    if ( url.i_port && asprintf( &p_entry->ppsz_values[KEY_PORT],
                                 "%d", url.i_port) == -1 )
    {
        p_entry->ppsz_values[KEY_PORT] = NULL;
        goto end;
    }
    if ( url.psz_path && !( p_entry->ppsz_values[KEY_PATH] =
                            strdup( url.psz_path ) ) )
        goto end;
    if ( url.psz_option )
    {
        char *p_savetpr;

        for ( const char *psz_option = strtok_r( url.psz_option, "&", &p_savetpr );
              psz_option != NULL;
              psz_option = strtok_r( NULL, "&", &p_savetpr ) )
        {
            enum vlc_keystore_key key;
            const char *psz_value;

            if ( !strncmp( psz_option, "realm=", strlen( "realm=" ) ) )
            {
                key = KEY_REALM;
                psz_value = psz_option + strlen( "realm=" );
            }
            else if ( !strncmp( psz_option, "authtype=", strlen( "authtype=" ) ) )
            {
                key = KEY_AUTHTYPE;
                psz_value = psz_option + strlen( "authtype=" );
            }
            else
                psz_value = NULL;

            if ( psz_value != NULL )
            {
                p_entry->ppsz_values[key] = vlc_b64_decode( psz_value );
                if ( !p_entry->ppsz_values[key] )
                    goto end;
            }
        }
    }

    i_ret = VLC_SUCCESS;

end:
    vlc_UrlClean( &url );
    if ( i_ret )
    {
        free( p_entry->ppsz_values[KEY_PROTOCOL] );
        free( p_entry->ppsz_values[KEY_USER] );
        free( p_entry->ppsz_values[KEY_SERVER] );
        free( p_entry->ppsz_values[KEY_PORT] );
        free( p_entry->ppsz_values[KEY_PATH] );
        free( p_entry->ppsz_values[KEY_REALM] );
        free ( p_entry->ppsz_values[KEY_AUTHTYPE] );
    }
    return i_ret;
}

static int
kwallet_network_wallet( vlc_keystore* p_keystore )
{
    int i_ret;
    vlc_keystore_sys* p_sys = p_keystore->p_sys;
    sd_bus* bus = p_sys->bus;
    sd_bus_message* msg = NULL;
    sd_bus_error err = SD_BUS_ERROR_NULL;
    char *psz_reply = NULL ;

    i_ret = sd_bus_call_method( bus,
                               KWALLET_SERVICE,
                               KWALLET_PATH,
                               psz_kwallet_interface,
                               "networkWallet",
                               &err,
                               &msg,
                               "");
    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_network_wallet : Failed to issue methode call: error (%s)", err.message );
        goto end;
    }

    i_ret = sd_bus_message_read_basic( msg, 's', &psz_reply );
    if( i_ret < 0 )
    {
        msg_Dbg( p_keystore, "kwallet_network_wallet: Failed to parse response message: %s\n", strerror( -i_ret ) );
        goto end;
    }
    p_sys->psz_wallet = strdup( psz_reply );

    if ( !p_sys->psz_wallet )
    {
        i_ret = VLC_ENOMEM;
        goto end;
    }

end:
    sd_bus_error_free(&err);
    if(msg)
        sd_bus_message_unref(msg);
    return i_ret;
}

static int
kwallet_is_enabled( vlc_keystore* p_keystore, int *i_is_enabled )
{
    int i_ret;
    vlc_keystore_sys* p_sys = p_keystore->p_sys;
    sd_bus* bus = p_sys->bus;
    sd_bus_message* msg = NULL;
    sd_bus_error err = SD_BUS_ERROR_NULL;

    i_ret = sd_bus_call_method( bus,
                               "org.freedesktop.DBus",
                               "/org/freedesktop/DBus",
                               "org.freedesktop.DBus",
                               "NameHasOwner",
                               &err,
                               &msg,
                               "s",
                               psz_kwallet_interface);
    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_is_enabled: Failed to issue methode call: error (%s)", err.message );
        goto end;
    }

    i_ret = sd_bus_message_read_basic( msg, 'b', i_is_enabled );
    if( i_ret < 0 )
    {
        msg_Dbg( p_keystore, "kwallet_is_enabled: Failed to parse response message: %s\n", strerror( -i_ret ) );
        goto end;
    }

    i_ret = VLC_SUCCESS;

end:
    sd_bus_error_free(&err);
    if(msg)
        sd_bus_message_unref(msg);
    return i_ret;
}

static int
vlc_dbus_init( vlc_keystore* p_keystore )
{
    vlc_keystore_sys* p_sys = p_keystore->p_sys;
    sd_bus *bus = p_sys->bus;
    int i_ret;

    /* DBus Connection */
    i_ret = sd_bus_open_user( &p_sys->bus );
    if ( i_ret < 0 )
    {
        msg_Dbg( p_keystore, "vlc_dbus_init : "
                 "Connection error to session bus (%s)", strerror(-i_ret));
        return VLC_EGENERIC;
    }

    /* requesting name */
    for( unsigned i = 0; i <= 99; ++i )
    {
        char psz_dbus_name[strlen( KWALLET_APP_ID ) + strlen( DBUS_INSTANCE_PREFIX ) + 5];

        sprintf( psz_dbus_name, "%s.%s_%02u", KWALLET_APP_ID, DBUS_INSTANCE_PREFIX, i );
        i_ret = sd_bus_request_name( p_sys->bus, psz_dbus_name, 0);
        if ( i_ret < 0 )
        {
            msg_Dbg( p_keystore, "vlc_dbus_init : dbus_bus_request_name :" " error (%s)", strerror(-i_ret) );
        }
        else
        {
            p_sys->psz_app_id = strdup( psz_dbus_name );
            if ( !p_sys->psz_app_id )
                goto error;
            break;
        }

    }
    if ( p_sys->psz_app_id == NULL )
    {
        msg_Dbg( p_keystore, "vlc_dbus_init: Too many kwallet instances" );
        goto error;
    }

    int i_is_enabled;
    /* check to see if any kwallet service is enabled */
    if ( kwallet_is_enabled( p_keystore, &i_is_enabled ) )
    {
        msg_Dbg( p_keystore, "vlc_dbus_init: error while checking if kwallet is enabled" );
        goto error;
    }

    if ( !i_is_enabled )
    {
        msg_Dbg( p_keystore, "vlc_dbus_init: kwallet is not running" );
    }

    /* getting the name of the wallet assigned to network passwords */
    if ( kwallet_network_wallet( p_keystore ) < 0 )
    {
        msg_Dbg(p_keystore, "vlc_dbus_init: kwallet_network_wallet has failed");
        goto error;
    }

    return VLC_SUCCESS;

error:
    FREENULL( p_sys->psz_app_id );
    sd_bus_flush( bus );
    sd_bus_close( bus );
    return VLC_EGENERIC;
}

static int
kwallet_has_folder( vlc_keystore* p_keystore, const char* psz_folder_name, int *b_has_folder )
{
    int i_ret;
    vlc_keystore_sys* p_sys = p_keystore->p_sys;
    sd_bus *bus = p_sys->bus;
    sd_bus_message *msg;
    sd_bus_error err = SD_BUS_ERROR_NULL;

    i_ret = sd_bus_call_method( bus,
                                KWALLET_SERVICE,
                                KWALLET_PATH,
                                psz_kwallet_interface,
                                "hasFolder",
                                &err,
                                &msg,
                                "iss",
                                p_sys->i_handle,
                                psz_folder_name,
                                p_sys->psz_app_id);
    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_has_folder : Failed to issue methode call: error (%s)", err.message );
        goto end;
    }

    i_ret = sd_bus_message_read_basic(msg, 'b', b_has_folder);
    if( i_ret < 0 )
        msg_Err( p_keystore, "kwallet_has_folder: Failed to parse response message: %s\n", strerror(-i_ret) );

end:
    sd_bus_error_free(&err);
    if(msg)
        sd_bus_message_unref(msg);
    return i_ret;
}

static int
kwallet_create_folder( vlc_keystore* p_keystore, const char* psz_folder_name )
{
    int i_ret;
    vlc_keystore_sys* p_sys = p_keystore->p_sys;
    sd_bus *bus = p_sys->bus;
    sd_bus_message *msg;
    sd_bus_error err = SD_BUS_ERROR_NULL;
    int b_reply;

    i_ret = sd_bus_call_method( bus,
                                KWALLET_SERVICE,
                                KWALLET_PATH,
                                psz_kwallet_interface,
                                "createFolder",
                                &err,
                                &msg,
                                "iss",
                                p_sys->i_handle,
                                psz_folder_name,
                                p_sys->psz_app_id );
    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_create_folder : Failed to issue methode call: error (%s)", err.message );
        goto end;
    }

    i_ret = sd_bus_message_read_basic( msg, 'b', &b_reply );
    if( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_create_folder: Failed to parse response message: %s\n", strerror(-i_ret));
        goto end;
    }

    if( !b_reply )
    {
        msg_Err( p_keystore, "kwallet_create_folder : Could not create folder" );
        goto end;
    }

end:
    sd_bus_error_free(&err);
    if(msg)
        sd_bus_message_unref(msg);
    return i_ret;
}

static int
kwallet_open( vlc_keystore* p_keystore )
{
    int i_ret;
    vlc_keystore_sys* p_sys = p_keystore->p_sys;
    sd_bus* bus = p_sys->bus;
    sd_bus_message* msg = NULL;
    sd_bus_error err = SD_BUS_ERROR_NULL;

    unsigned long long ull_win_id = 0;
    int b_has_folder;

    i_ret = sd_bus_call_method( bus,
                                KWALLET_SERVICE,
                                KWALLET_PATH,
                                psz_kwallet_interface,
                                "open",
                                &err,
                                &msg,
                                "sxs",
                                p_sys->psz_wallet,
                                ull_win_id,
                                p_sys->psz_app_id );
    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_open : Failed to issue new method call: error (%s)", err.message);
        goto end;
    }

    sd_bus_message_read_basic( msg, 'i', &p_sys->i_handle);
    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_open: Failed to parse response message: %s\n", strerror(-i_ret) );
        goto end;
    }

    /* opening the vlc password folder == VLC_KEYSTORE_NAME */
    if ( kwallet_has_folder( p_keystore, psz_folder, &b_has_folder ) < 0 )
        goto end;
    if ( !b_has_folder )
    {
        if ( kwallet_create_folder( p_keystore, psz_folder ) < 0 )
        {
            msg_Err( p_keystore, "kwallet_open : could not create folder %s",
                     psz_folder );
            goto end;
        }
    }

end:
    sd_bus_error_free( &err );
    if(msg)
        sd_bus_message_unref( msg );

    return i_ret;
}

static int
kwallet_has_entry( vlc_keystore* p_keystore, char* psz_entry_name, int *b_has_entry )
{
    int i_ret;
    vlc_keystore_sys* p_sys = p_keystore->p_sys;
    sd_bus* bus = p_sys->bus;
    sd_bus_message* msg = NULL;
    sd_bus_error err = SD_BUS_ERROR_NULL;

    i_ret = sd_bus_call_method( bus,
                                KWALLET_SERVICE,
                                KWALLET_PATH,
                                psz_kwallet_interface,
                                "hasEntry",
                                &err,
                                &msg,
                                "iss",
                                p_sys->i_handle,
                                psz_folder,
                                psz_entry_name,
                                p_sys->psz_app_id );

    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_has_entry : Failed to issue new method call: error (%s)", err.message);
        goto end;
    }

    sd_bus_message_read_basic( msg, 'b', b_has_entry);
    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_has_entry: Failed to parse response message: %s\n", strerror(-i_ret) );
        goto end;
    }

end:
    sd_bus_error_free( &err );
    if(msg)
        sd_bus_message_unref( msg );

    return i_ret;
}

static int
kwallet_write_password( vlc_keystore* p_keystore, char* psz_entry_name, const char* psz_secret )
{
    int i_ret;
    int i_reply;
    vlc_keystore_sys* p_sys = p_keystore->p_sys;
    sd_bus* bus = p_sys->bus;
    sd_bus_message* msg = NULL;
    sd_bus_error err = SD_BUS_ERROR_NULL;

    i_ret = sd_bus_call_method( bus,
                                KWALLET_SERVICE,
                                KWALLET_PATH,
                                psz_kwallet_interface,
                                "writePassword",
                                &err,
                                &msg,
                                "issss",
                                p_sys->i_handle,
                                psz_folder,
                                psz_entry_name,
                                psz_secret,
                                p_sys->psz_app_id );
    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_write_password : Failed to issue new method call: error (%s)", err.message);
        goto end;
    }

    sd_bus_message_read_basic( msg, 'b', &i_reply);
    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_write_password: Failed to parse response message: %s\n", strerror(-i_ret) );
        goto end;
    }

    i_ret = VLC_SUCCESS;
    if ( !i_reply )
        i_ret = VLC_EGENERIC;

end:
    sd_bus_error_free( &err );
    if(msg)
        sd_bus_message_unref( msg );

    return i_ret;
}

static int
kwallet_remove_entry( vlc_keystore* p_keystore, char* psz_entry_name )
{
    int i_ret;
    int i_reply;
    int b_has_entry = 0;

    vlc_keystore_sys* p_sys = p_keystore->p_sys;
    sd_bus* bus = p_sys->bus;
    sd_bus_message* msg = NULL;
    sd_bus_error err = SD_BUS_ERROR_NULL;

    if ( kwallet_has_entry( p_keystore, psz_entry_name, &b_has_entry ) < 0 )
    {
        msg_Err( p_keystore, "kwallet_remove_entry : kwallet_has_entry failed" );
        return VLC_EGENERIC;
    }
    if ( !b_has_entry )
    {
        msg_Err( p_keystore, "kwallet_remove_entry : there is no such entry :"
                "%s", psz_entry_name );
        return VLC_EGENERIC;
    }

    i_ret = sd_bus_call_method( bus,
                                KWALLET_SERVICE,
                                KWALLET_PATH,
                                psz_kwallet_interface,
                                "removeEntry",
                                &err,
                                &msg,
                                "isss",
                                p_sys->i_handle,
                                psz_folder,
                                psz_entry_name,
                                p_sys->psz_app_id );
    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_remove_entry : Failed to issue new method call: error (%s)", err.message);
        goto end;
    }

    sd_bus_message_read_basic( msg, 'b', &i_reply);
    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "kwallet_remove_entry: Failed to parse response message: %s\n", strerror(-i_ret) );
        goto end;
    }

    i_ret = VLC_SUCCESS;
    if ( !i_reply )
        i_ret = VLC_EGENERIC;

end:
    sd_bus_error_free( &err );
    if(msg)
        sd_bus_message_unref( msg );

    return i_ret;
}

static vlc_keystore_entry*
kwallet_read_password_list( vlc_keystore* p_keystore, char* psz_entry_name,
                            unsigned int* pi_count )
{
    int i_ret;

    vlc_keystore_sys* p_sys = p_keystore->p_sys;
    sd_bus* bus = p_sys->bus;
    sd_bus_message* msg = NULL;
    sd_bus_error err = SD_BUS_ERROR_NULL;
    vlc_keystore_entry* p_entries = NULL;
    size_t i_size;
    uint8_t* p_secret_decoded = NULL;
    char* p_reply;
    char* p_secret;
    keystore_vector_t vec;
    vlc_vector_init(&vec);

    *pi_count = 0;
    i_ret = sd_bus_call_method( bus,
                                KWALLET_SERVICE,
                                KWALLET_PATH,
                                psz_kwallet_interface,
                                "readPasswordList",
                                &err,
                                &msg,
                                "isss",
                                p_sys->i_handle,
                                psz_folder,
                                psz_entry_name,
                                p_sys->psz_app_id );
    if ( i_ret < 0 )
    {
        msg_Err( p_keystore, "readPasswordList : Failed to issue new method call: error (%s)", err.message );
        goto error;
    }

    i_ret = sd_bus_message_enter_container( msg, 'a', "{sv}" );
    if ( i_ret < 0 )
        goto error;

    for( ;; )
    {
        vlc_keystore_entry entry;
        i_ret = sd_bus_message_enter_container( msg, 'e', "sv" );
        if( i_ret < 0 )
            goto error;
        if ( i_ret == 0 )
            break;
        i_ret = sd_bus_message_read( msg, "s", &p_reply );
        if( i_ret < 0 )
            goto error;
        i_ret = sd_bus_message_enter_container(msg, 'v', "s" );
        if( i_ret < 0 )
            goto error;
        i_ret = sd_bus_message_read( msg, "s", &p_secret );
        if( i_ret < 0 )
            goto error;
        i_size = vlc_b64_decode_binary( &p_secret_decoded, p_secret);
        if ( key2values( p_reply, &entry ) )
            goto error;
        if ( ( vlc_keystore_entry_set_secret( &entry,
                                              p_secret_decoded,
                                              i_size ) ) )
            goto error;
        i_ret = sd_bus_message_exit_container(msg);
        if (i_ret < 0)
            goto error;
        i_ret = sd_bus_message_exit_container(msg);
        if (i_ret < 0)
            goto error;
        if(!vlc_vector_push(&vec, entry))
            goto error;
        free(p_secret_decoded);
    }

    p_entries = malloc(vec.size * sizeof(vlc_keystore_entry));
    if(!p_entries)
        goto error;
    memcpy(p_entries, vec.data, vec.size * sizeof(vlc_keystore_entry));
    if(!p_entries)
        goto error;
    *pi_count = vec.size;
    vlc_vector_destroy( &vec );

    sd_bus_error_free( &err );
    if(msg)
        sd_bus_message_unref( msg );

    return p_entries;

error:
    vlc_vector_destroy( &vec );
    free( p_entries );
    free( p_secret_decoded );
    *pi_count = 0;
    sd_bus_error_free( &err );
    if(msg)
        sd_bus_message_unref( msg );
    return NULL;
}


static int
Store( vlc_keystore* p_keystore, const char *const ppsz_values[KEY_MAX],
       const uint8_t* p_secret, size_t i_secret_len, const char* psz_label )
{
    char* psz_key;
    char* psz_b64_secret;

    (void)psz_label;

    psz_key = values2key( ppsz_values, false );
    if ( !psz_key )
        return VLC_ENOMEM;

    psz_b64_secret = vlc_b64_encode_binary( p_secret, i_secret_len );
    if ( !psz_b64_secret )
        return VLC_ENOMEM;

    if ( kwallet_write_password( p_keystore, psz_key, psz_b64_secret ) )
    {
        free( psz_b64_secret );
        free( psz_key );
        return VLC_EGENERIC;
    }

    free( psz_b64_secret );
    free( psz_key );

    return VLC_SUCCESS;
}

static unsigned int
Find( vlc_keystore* p_keystore, const char *const ppsz_values[KEY_MAX],
      vlc_keystore_entry** pp_entries )
{
    char* psz_key;
    unsigned int i_count = 0;
    vlc_keystore_entry* p_entries;

    psz_key = values2key( ppsz_values, true );
    if ( !psz_key )
        return i_count;
    p_entries = kwallet_read_password_list( p_keystore, psz_key, &i_count );
    if ( !p_entries )
    {
        free( psz_key );
        return 0;
    }

    *pp_entries = p_entries;
    free( psz_key );
    return i_count;
}

static unsigned int
Remove( vlc_keystore* p_keystore, const char* const ppsz_values[KEY_MAX] )
{
    char* psz_key;
    vlc_keystore_entry* p_entries;
    unsigned i_count = 0;

    psz_key = values2key( ppsz_values, true );
    if ( !psz_key )
        return 0;

    p_entries = kwallet_read_password_list( p_keystore, psz_key, &i_count );
    if ( !p_entries )
    {
        free( psz_key );
        return 0;
    }

    free( psz_key );

    for ( unsigned int i = 0 ; i < i_count ; ++i )
    {
        psz_key = values2key( ( const char* const* )p_entries[i].ppsz_values, false );
        if ( !psz_key )
        {
            vlc_keystore_release_entries( p_entries, i_count );
            return i;
        }

        if ( kwallet_remove_entry( p_keystore, psz_key ) )
        {
            vlc_keystore_release_entries( p_entries, i_count );
            free( psz_key );
            return i;
        }
        for ( int inc = 0 ; inc < KEY_MAX ; ++inc )
            free( p_entries[i].ppsz_values[inc] );
        free( p_entries[i].p_secret );
        free( psz_key );
    }

    free( p_entries );

    return i_count;
}

static void
Close( vlc_object_t* p_this )
{
    vlc_keystore *p_keystore = ( vlc_keystore* )p_this;
    vlc_keystore_sys* p_sys = p_keystore->p_sys;
    sd_bus *bus = p_sys->bus;

    sd_bus_flush( bus );
    sd_bus_close( bus );
    free( p_keystore->p_sys->psz_app_id );
    free( p_keystore->p_sys->psz_wallet );
    free( p_keystore->p_sys );
}

static int
Open( vlc_object_t* p_this )
{
    vlc_keystore *p_keystore = ( vlc_keystore* )p_this;

    p_keystore->p_sys = calloc( 1, sizeof( vlc_keystore_sys ) );
    if ( !p_keystore->p_sys)
        return VLC_ENOMEM;

    if ( vlc_dbus_init( p_keystore ) < 0 )
    {
        msg_Dbg( p_keystore, "vlc_dbus_init failed" );
        return VLC_EGENERIC;
    }

    if ( kwallet_open( p_keystore ) < 0 )
    {
        msg_Dbg( p_keystore, "kwallet_open failed" );
        free( p_keystore->p_sys );
        return VLC_EGENERIC;
    }

    p_keystore->pf_store = Store;
    p_keystore->pf_find = Find;
    p_keystore->pf_remove = Remove;

    return VLC_SUCCESS;
}
