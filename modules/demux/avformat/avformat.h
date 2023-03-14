/*****************************************************************************
 * avformat.h: muxer and demuxer using libavformat
 *****************************************************************************
 * Copyright (C) 2001-2008 VLC authors and VideoLAN
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

void LibavcodecCallback( void *p_opaque, int i_level,
                             const char *psz_format, va_list va );

/* Demux module */
int  avformat_OpenDemux ( vlc_object_t * );
void avformat_CloseDemux( vlc_object_t * );

/* Mux module */
int  avformat_OpenMux ( vlc_object_t * );
void avformat_CloseMux( vlc_object_t * );

#define MUX_TEXT N_("Avformat mux")
#define MUX_LONGTEXT N_("Force use of a specific avformat muxer.")
#define FORMAT_TEXT N_( "Format name" )
#define FORMAT_LONGTEXT N_( "Internal libavcodec format name" )
