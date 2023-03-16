// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * stream_output.h : internal stream output
 *****************************************************************************
 * Copyright (C) 2002-2005 VLC authors and VideoLAN
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *          Laurent Aimar <fenrir@via.ecp.fr>
 *          Eric Petit <titer@videolan.org>
 *          Jean-Paul Saman <jpsaman #_at_# m2x.nl>
 ***************************************************************************/

#ifndef VLC_SRC_STREAMOUT_H
# define VLC_SRC_STREAMOUT_H 1

# include <vlc_sout.h>
# include <vlc_network.h>

/** Stream output instance */
sout_stream_t *sout_NewInstance( vlc_object_t *, const char * );
#define sout_NewInstance(a,b) sout_NewInstance(VLC_OBJECT(a),b)

sout_packetizer_input_t *sout_InputNew( sout_stream_t *, const es_format_t * );
int sout_InputDelete( sout_stream_t *, sout_packetizer_input_t * );
int sout_InputSendBuffer( sout_stream_t *, sout_packetizer_input_t *,
                          block_t * );

enum sout_input_query_e
{
    SOUT_INPUT_SET_SPU_HIGHLIGHT, /* arg1=const vlc_spu_highlight_t *, can fail */
};
int  sout_InputControl( sout_stream_t *, sout_packetizer_input_t *,
                        int i_query, ... );
void sout_InputFlush( sout_stream_t *, sout_packetizer_input_t * );

#endif
