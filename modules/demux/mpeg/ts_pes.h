/*****************************************************************************
 * ts_pes.h: Transport Stream input module for VLC.
 *****************************************************************************
 * Copyright (C) 2004-2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef VLC_TS_PES_H
#define VLC_TS_PES_H

#define BLOCK_FLAG_SOURCE_RANDOM_ACCESS (1 << BLOCK_FLAG_PRIVATE_SHIFT)

typedef struct
{
    vlc_object_t *p_obj;
    void *priv;
    void(*pf_parse)(vlc_object_t *, void *, block_t *, uint32_t, stime_t );
} ts_pes_parse_callback;

bool ts_pes_Drain( ts_pes_parse_callback *cb, ts_stream_t *p_pes );

bool ts_pes_Gather( ts_pes_parse_callback *cb,
                    ts_stream_t *p_pes, block_t *p_pkt,
                    bool b_unit_start, bool b_valid_scrambling,
                    stime_t i_append_pcr );


#endif
