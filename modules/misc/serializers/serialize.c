/*****************************************************************************
 * serialize.c
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
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
#include <vlc_plugin.h>
#include <vlc_input_item.h>
#include <vlc_meta.h>
#include <vlc_es.h>
#include <vlc_memstream.h>
#include <vlc_serializer.h>

/***************************************************************************
 * Serializable types
 ***************************************************************************/
int serialize_es_format   ( type_serializer_t * );
int deserialize_es_format ( type_serializer_t * );

static int SetEsFormatSerializer( vlc_object_t *p_this )
{
    type_serializer_t *serialize = (type_serializer_t *)p_this;

    serialize->pf_serialize = serialize_es_format;
    serialize->pf_deserialize = deserialize_es_format;

    return VLC_SUCCESS;
}

vlc_module_begin()
    set_subcategory( SUBCAT_ADVANCED_MISC )

    add_submodule()
        set_description( N_("es_format_t serializer") )
        add_shortcut( "serialize-es" )
        set_capability( "serialize type", 0 )
        set_callback( SetEsFormatSerializer )

vlc_module_end()
