/*****************************************************************************
 * bdmv.c: Dummy bdmv demux - opens BluRays rips via index.bdmv
 *****************************************************************************
 * Copyright (C) 2018 VLC authors and VideoLAN
 *
 * Authors: Shaya Potter <spotter@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_access.h>
#include <vlc_url.h>
#include <assert.h>

#include "playlist.h"

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int ReadBR( stream_t *, input_item_node_t * );

static const char *StreamLocation( const stream_t *s )
{
    return s->psz_filepath ? s->psz_filepath : s->psz_url;
}

/*****************************************************************************
 * Import_BDMV: main import function
 *****************************************************************************/
int Import_BDMV( vlc_object_t *p_this )
{
    stream_t *p_stream = (stream_t *)p_this;

    if( !stream_HasExtension( p_stream, ".BDMV" ) )
        return VLC_EGENERIC;

    const char *psz_location = StreamLocation( p_stream );
    if( psz_location == NULL )
        return VLC_EGENERIC;

    size_t len = strlen( psz_location );
    if( len < 15 )
        return VLC_EGENERIC;

    const char *psz_probe;
    const char *psz_file = &psz_location[len - 10];
    /* Valid filenames are :
     *  - INDEX.BDMV
     */
    if( !strncasecmp( psz_file, "INDEX", 5 ) )
    {
        psz_probe = "INDX0200";
        p_stream->pf_readdir = ReadBR;
    }
    else
        return VLC_EGENERIC;

    const uint8_t *p_peek;
    ssize_t i_peek = vlc_stream_Peek( p_stream->s, &p_peek, 8 );
    if( i_peek < 8 || memcmp( p_peek, psz_probe, 8 ) )
        return VLC_EGENERIC;

    p_stream->pf_control = PlaylistControl;

    return VLC_SUCCESS;
}

static int ReadBR( stream_t *p_stream, input_item_node_t *node )
{
    const char *psz_loc = StreamLocation(p_stream);

    // 10 character in INDEX.BDMV, 5 character in BDMV/, subtract 15
    char *psz_url = strndup( psz_loc, strlen( psz_loc) - 15 );
    if( !psz_url )
        return VLC_ENOMEM;

    input_item_t *p_input = input_item_New( psz_url, psz_url );
    input_item_AddOption( p_input, "demux=bluray", VLC_INPUT_OPTION_TRUSTED );
    input_item_node_AppendItem( node, p_input );
    input_item_Release( p_input );

    free( psz_url );

    return VLC_SUCCESS;
}
