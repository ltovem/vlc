/*****************************************************************************
 * acoustid.c: AcoustId webservice parser
 *****************************************************************************
 * Copyright (C) 2012 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
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
#include <vlc_messages.h>

#include "json_helper.h"
#include "acoustid.h"

/*****************************************************************************
 * Requests lifecycle
 *****************************************************************************/
void acoustid_result_release( acoustid_result_t * r )
{
    free( r->psz_id );
    for ( unsigned int i=0; i<r->recordings.count; i++ )
    {
        free( r->recordings.p_recordings[ i ].psz_artist );
        free( r->recordings.p_recordings[ i ].psz_title );
    }
    free( r->recordings.p_recordings );
}

static void parse_artists( struct json_value node, acoustid_mb_result_t *record )
{
    /* take only main */
    if ( node.type != JSON_ARRAY || node.array.size < 1 )
        return;
    record->psz_artist = json_dupstring( &node.array.entries[ 0 ], "name" );
}

static void parse_recordings( vlc_object_t *p_obj, struct json_object *object, acoustid_result_t *p_result )
{
    const struct json_value *value = json_get(object, "recordings");
    if ( value->type != JSON_ARRAY ) return;
    p_result->recordings.p_recordings = calloc( value->array.size, sizeof(acoustid_mb_result_t) );
    if ( ! p_result->recordings.p_recordings ) return;
    p_result->recordings.count = value->array.size;

    for( unsigned int i=0; i<value->array.size; i++ )
    {
        acoustid_mb_result_t *record = & p_result->recordings.p_recordings[ i ];
        struct json_value *recordnode = &value->array.entries[ i ];
        if ( !recordnode || recordnode->type != JSON_OBJECT )
            break;
        record->psz_title = json_dupstring( recordnode, "title" );
        struct json_value id = json_getbyname( &recordnode->object, "id" );
        if ( id.type == JSON_STRING )
        {
            size_t i_len = strlen( id.string );
            i_len = __MIN( i_len, MB_ID_SIZE );
            memcpy( record->s_musicbrainz_id, id.string, i_len );
        }
        parse_artists( json_getbyname( &recordnode->object, "artists" ), record );
        msg_Dbg( p_obj, "recording %d title %s %36s %s", i, record->psz_title,
                 record->s_musicbrainz_id, record->psz_artist );
    }
}

static bool ParseJson( vlc_object_t *p_obj, const void *p_buffer, acoustid_results_t *p_results )
{
    struct sys_json jsdata;
    struct json_object json;
    jsdata.logger = p_obj->logger;
    jsdata.input = (const char *)p_buffer;
    jsdata.size = strlen(jsdata.input);
    jsdata.json_read = Read;

    int val = json_parse(&jsdata, &json);
    if (val) {
        msg_Dbg( p_obj, "error: could not parse json!");
        return false;
    }

    struct json_value node = json_getbyname( &json, "status" );
    if ( node.type != JSON_STRING )
    {
        msg_Warn( p_obj, "status node not found or invalid" );
        json_free(&json);
        return false;
    }
    if ( strcmp( node.string, "ok" ) != 0 )
    {
        msg_Warn( p_obj, "Bad request status" );
        json_free(&json);
        return false;
    }
    node = json_getbyname( &json, "results" );
    if ( node.type != JSON_ARRAY )
    {
        msg_Warn( p_obj, "Bad results array or no results" );
        json_free(&json);
        return false;
    }
    p_results->p_results = calloc( node.array.size, sizeof(acoustid_result_t) );
    if ( ! p_results->p_results ) {
        json_free(&json);
        return false;
    }
    p_results->count = node.array.size;
    for( unsigned int i=0; i<node.array.size; i++ )
    {
        struct json_value *resultnode = &node.array.entries[i];
        if ( resultnode->type == JSON_OBJECT )
        {
            acoustid_result_t *p_result = & p_results->p_results[i];
            struct json_value value = json_getbyname( &resultnode->object, "score" );
            if ( value.type == JSON_NUMBER )
                p_result->d_score = value.number;
            p_result->psz_id = json_dupstring( resultnode, "id" );
            parse_recordings( p_obj, &resultnode->object, p_result );
        }
    }
    json_free(&json);
    return true;
}

int acoustid_lookup_fingerprint( const acoustid_config_t *p_cfg, acoustid_fingerprint_t *p_data )
{
    if ( !p_data->psz_fingerprint )
        return VLC_SUCCESS;

    char *psz_url;
    if( p_cfg->psz_server )
    {
        if( unlikely(asprintf( &psz_url, "https://%s/v2/lookup"
                               "?client=%s"
                               "&meta=recordings+tracks+usermeta+releases"
                               "&duration=%d"
                               "&fingerprint=%s",
                               p_cfg->psz_server,
                               p_cfg->psz_apikey ? p_cfg->psz_apikey : "",
                               p_data->i_duration,
                               p_data->psz_fingerprint ) < 1 ) )
             return VLC_EGENERIC;
    }
    else /* Use VideoLAN anonymized requests proxy */
    {
        if( unlikely(asprintf( &psz_url, "https://" ACOUSTID_ANON_SERVER
                               ACOUSTID_ANON_SERVER_PATH
                               "?meta=recordings+tracks+usermeta+releases"
                               "&duration=%d"
                               "&fingerprint=%s",
                               p_data->i_duration,
                               p_data->psz_fingerprint ) < 1 ) )
             return VLC_EGENERIC;
    }

    msg_Dbg( p_cfg->p_obj, "Querying AcoustID from %s", psz_url );
    void *p_buffer = json_retrieve_document( p_cfg->p_obj, psz_url );
    free( psz_url );
    if( !p_buffer )
        return VLC_EGENERIC;

    if ( ParseJson( p_cfg->p_obj, p_buffer, & p_data->results ) )
        msg_Dbg( p_cfg->p_obj, "results count == %d", p_data->results.count );
    else
        msg_Dbg( p_cfg->p_obj, "No results" );
    free( p_buffer );

    return VLC_SUCCESS;
}
