// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * streams.h
 *****************************************************************************
 * Copyright (C) 2001-2005, 2015 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef VLC_MPEG_STREAMS_H_
#define VLC_MPEG_STREAMS_H_

typedef struct
{
    uint16_t        i_pid;

    uint8_t         i_stream_type;
    uint8_t         i_continuity_counter;
    bool            b_discontinuity;

} tsmux_stream_t;

typedef struct
{
    int             i_stream_id; /* keep as int for drac */

    /* Specific to mpeg4 in mpeg2ts */
    int             i_es_id;

    /* language is iso639-2T */
    size_t          i_langs;
    uint8_t         *lang;
} pesmux_stream_t;

#endif
