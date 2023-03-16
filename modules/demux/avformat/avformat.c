// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * avformat.c: demuxer and muxer using libavformat library
 *****************************************************************************
 * Copyright (C) 1999-2008 VLC authors and VideoLAN
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Gildas Bazin <gbazin@videolan.org>
 *****************************************************************************/

#ifndef MERGE_FFMPEG
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_demux.h>

#include "avformat.h"
#include "../../codec/avcodec/avcommon.h"

vlc_module_begin ()
#endif /* MERGE_FFMPEG */
    add_shortcut( "ffmpeg", "avformat" )
    add_file_extension("rm")
    set_subcategory( SUBCAT_INPUT_DEMUX )
    set_description( N_("Avformat demuxer" ) )
    set_shortname( N_("Avformat") )
    set_capability( "demux", 2 )
    set_callbacks( avformat_OpenDemux, avformat_CloseDemux )
    set_section( N_("Demuxer"), NULL )
    add_string( "avformat-format", NULL, FORMAT_TEXT, FORMAT_LONGTEXT )
    add_string( "avformat-options", NULL, AV_OPTIONS_TEXT, AV_OPTIONS_LONGTEXT )

#ifdef ENABLE_SOUT
    /* mux submodule */
    add_submodule ()
    add_shortcut( "ffmpeg", "avformat" )
    set_description( N_("Avformat muxer" ) )
    set_capability( "sout mux", 2 )
    set_section( N_("Muxer"), NULL )
    add_string( "sout-avformat-mux", NULL, MUX_TEXT, MUX_LONGTEXT )
    add_string( "sout-avformat-options", NULL, AV_OPTIONS_TEXT, AV_OPTIONS_LONGTEXT )
    add_bool( "sout-avformat-reset-ts", false, AV_RESET_TS_TEXT, AV_RESET_TS_LONGTEXT )
    set_callbacks( avformat_OpenMux, avformat_CloseMux )
#endif
#ifndef MERGE_FFMPEG
vlc_module_end ()
#endif
