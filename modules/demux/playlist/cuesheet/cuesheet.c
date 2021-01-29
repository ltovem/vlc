/*****************************************************************************
 * cuesheet.c : cue sheet playlist import format
 *****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * Authors: Kartik Ohri <kartikohri13@gmail.com>
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

#include <string.h>

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_access.h>
#include <vlc_url.h>

#include "cuesheet/cuesheet.h"
#include "../playlist.h"

int Import_Cue_Sheet(vlc_object_t *);
static int ReadDir(stream_t *, input_item_node_t *);

int Import_Cue_Sheet( vlc_object_t *p_this )
{
    stream_t *p_demux = (stream_t *) p_this;

    if ( !stream_IsMimeType ( p_demux->s, "application/x-cue" )
    && !stream_HasExtension( p_demux->s, ".cue" ))
        return VLC_EGENERIC;

    p_demux->pf_control = access_vaDirectoryControlHelper;
    p_demux->pf_readdir = ReadDir;
    return VLC_SUCCESS;
}

/**
 * free should not be called on strings allocated by rust compiler. There is no
 * way to ensure this without duplicating the string before passing it to
 * input_item_t.
 */

static int ReadDir(stream_t* p_demux, input_item_node_t* p_subitems)
{
    return read_dir(p_demux->s, p_subitems, p_demux->psz_url);
}

vlc_module_begin()
    add_shortcut( "playlist" )
    set_category( CAT_INPUT )
    set_subcategory( SUBCAT_INPUT_DEMUX )
    set_description ( N_("Cue Sheet importer") )
    set_capability ( "stream_filter", 320 )
    set_callback ( Import_Cue_Sheet )
vlc_module_end()
