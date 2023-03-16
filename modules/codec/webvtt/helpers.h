// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * helpers.h: WEBVTT helper
 *****************************************************************************
 * Copyright (C) 2020 VideoLabs, VLC authors and VideoLAN
 *****************************************************************************/
#ifndef WEBVTT_HELPERS_H
#define WEBVTT_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct webvtt_cueelements_s
{
    struct
    {
        const uint8_t *p_data;
        size_t i_data;
    } iden, sttg, payl;
};

static inline size_t
WEBVTT_Pack_CueElementsGetNewSize( const struct webvtt_cueelements_s *els )
{
    return 8 +
           (els->iden.i_data ? (els->iden.i_data + 8) : 0) +
           (els->sttg.i_data ? (els->sttg.i_data + 8) : 0) +
           els->payl.i_data + 8;
}

static inline void
WEBVTT_Pack_CueElements( const struct webvtt_cueelements_s *els,
                         uint8_t *p_dst )
{
    size_t i_total = WEBVTT_Pack_CueElementsGetNewSize( els );
    /* root container */
    SetDWBE( &p_dst[0], i_total );
    memcpy(  &p_dst[4], "vttc", 4 );
    p_dst += 8;
    /* id child */
    if( els->iden.i_data )
    {
        SetDWBE( &p_dst[0], 8 + els->iden.i_data );
        memcpy(  &p_dst[4], "iden", 4 );
        memcpy(  &p_dst[8], els->iden.p_data, els->iden.i_data );
        p_dst += 8 + els->iden.i_data;
    }
    /* cue settings child */
    if( els->sttg.i_data )
    {
        SetDWBE( &p_dst[0], 8 + els->sttg.i_data );
        memcpy(  &p_dst[4], "sttg", 4 );
        memcpy(  &p_dst[8], els->sttg.p_data, els->sttg.i_data );
        p_dst += 8 + els->sttg.i_data;
    }
    /* text */
    SetDWBE( &p_dst[0], 8 + els->payl.i_data );
    memcpy(  &p_dst[4], "payl", 4 );
    memcpy(  &p_dst[8], els->payl.p_data, els->payl.i_data );
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
