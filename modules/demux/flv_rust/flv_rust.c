/*****************************************************************************
 * rust-flv.c: zlib decompression module for VLC
 *****************************************************************************
 * Copyright © 2020 Johan 'Enulp' Pardo
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

#include <stdlib.h>
#include <string.h>

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_stream.h>
#include <vlc_demux.h>
#include "../../../src/vlccore_sys/vlccore_helper.h"
#include "src/vlccore_sys/vlccore_sys.h"
#include "flv_rust/flv_rust.h"

static int Open(vlc_object_t *obj)
{
    demux_t *p_demux = (demux_t*)obj;
    p_demux->p_sys = flv_open(p_demux->s);
    if (p_demux->p_sys == NULL)
        return VLC_EGENERIC;
    p_demux->pf_demux = vlcrs_demux_demux;
    p_demux->pf_control = vlcrs_demux_vaControlHelper;
    return VLC_SUCCESS;
}

static void Close (vlc_object_t *obj)
{
    demux_t *demux = (demux_t *)obj;
    vlcrs_demux_close(demux);
}

vlc_module_begin()
    set_category(CAT_INPUT)
    set_subcategory(SUBCAT_INPUT_STREAM_FILTER)
    set_capability("demux", 210)

    set_description(N_("flv_rust"))
    set_callbacks(Open, Close)
vlc_module_end()
