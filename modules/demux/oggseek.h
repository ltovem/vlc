// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * oggseek.h : ogg seeking functions for ogg demuxer vlc
 *****************************************************************************
 * Copyright (C) 2008 - 2010 Gabriel Finch <salsaman@gmail.com>
 *
 * Authors: Gabriel Finch <salsaman@gmail.com>
 * adapted from: http://lives.svn.sourceforge.net/viewvc/lives/trunk/lives-plugins
 * /plugins/decoders/ogg_theora_decoder.c
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

//#define OGG_SEEK_DEBUG 1
#ifdef OGG_SEEK_DEBUG
  #define OggDebug(code) code
  #define OggNoDebug(code)
#else
  #define OggDebug(code)
  #define OggNoDebug(code) code
#endif

#define PAGE_HEADER_BYTES 27

#define OGGSEEK_BYTES_TO_READ 8500
#define OGGSEEK_SERIALNO_MAX_LOOKUP_BYTES (OGGSEEK_BYTES_TO_READ * 25)

/* this is typedefed to demux_index_entry_t in ogg.h */
struct oggseek_index_entry
{
    demux_index_entry_t *p_next;
    /* value is highest granulepos for theora, sync frame for dirac */
    vlc_tick_t i_value;
    int64_t i_pagepos;
};

int     Oggseek_BlindSeektoAbsoluteTime ( demux_t *, logical_stream_t *, vlc_tick_t, bool );
int     Oggseek_BlindSeektoPosition ( demux_t *, logical_stream_t *, double f, bool );
int     Oggseek_SeektoAbsolutetime ( demux_t *, logical_stream_t *, vlc_tick_t );
const demux_index_entry_t *OggSeek_IndexAdd ( logical_stream_t *, vlc_tick_t, int64_t );
void    Oggseek_ProbeEnd( demux_t * );

void oggseek_index_entries_free ( demux_index_entry_t * );

int64_t oggseek_read_page ( demux_t * );
