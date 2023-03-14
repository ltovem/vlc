/*****************************************************************************
 * directory.c : Use access readdir to output folder content to playlist
 *****************************************************************************
 * Copyright (C) 2014 VLC authors and VideoLAN
 *
 * Authors: Julien 'Lta' BALLET <contact # lta . io >
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_demux.h>
#include <vlc_input_item.h>
#include <vlc_plugin.h>

static int ReadDir(demux_t *demux, input_item_node_t *node)
{
    return vlc_stream_ReadDir(demux->s, node);
}

static int Control(demux_t *demux, int query, va_list args)
{
    switch( query )
    {
        case DEMUX_GET_META:
        case DEMUX_GET_TYPE:
        {
            return vlc_stream_vaControl(demux->s, query, args);
        }
        case DEMUX_HAS_UNSUPPORTED_META:
        {
            *(va_arg( args, bool * )) = false;
            return VLC_SUCCESS;
        }
    }
    return VLC_EGENERIC;
}

static int Import_Dir( vlc_object_t *p_this )
{
    demux_t *p_demux = (demux_t *)p_this;

    if( p_demux->s->pf_readdir == NULL )
        return VLC_EGENERIC;
    if( p_demux->p_input_item == NULL )
        return VLC_ETIMEOUT;

    p_demux->pf_readdir = ReadDir;
    p_demux->pf_control = Control;
    assert(p_demux->pf_demux == NULL);

    return VLC_SUCCESS;
}

vlc_module_begin()
    set_subcategory( SUBCAT_INPUT_DEMUX )
    set_shortname( N_("Directory") )
    set_description( N_("Directory import") )
    add_shortcut( "directory" )
    set_capability( "demux", 10 )
    set_callback( Import_Dir )
vlc_module_end()
