// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * avparser.h
 *****************************************************************************
 * Copyright (C) 2015 VLC authors and VideoLAN
 *
 * Authors: Denis Charmet <typx@videolan.org>
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_codec.h>
#include <vlc_block.h>

#include "../codec/avcodec/avcodec.h"
#include "../codec/avcodec/avcommon.h"

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
int  avparser_OpenPacketizer ( vlc_object_t * );
void avparser_ClosePacketizer( vlc_object_t * );

#define AVPARSER_MODULE \
    set_subcategory( SUBCAT_SOUT_PACKETIZER )           \
    set_description( N_("avparser packetizer") )        \
    set_capability( "packetizer", 20 )                   \
    set_callbacks( avparser_OpenPacketizer, avparser_ClosePacketizer )
