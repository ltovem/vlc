// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * tables.h
 *****************************************************************************
 * Copyright (C) 2001-2005, 2015 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef VLC_MPEG_TABLES_H_
#define VLC_MPEG_TABLES_H_

#define MAX_SDT_DESC 64

typedef struct
{
    tsmux_stream_t ts;
    int i_netid;
    struct
    {
        char *psz_provider; /* provider in UTF8 */
        char *psz_service_name; /* program in UTF8 */
    } desc[MAX_SDT_DESC];
} sdt_psi_t;

block_t * WritePSISection( dvbpsi_psi_section_t* p_section );

void BuildPAT( dvbpsi_t *p_dvbpsi,
               void *p_opaque, PEStoTSCallback pf_callback,
               int i_tsid, int i_pat_version_number,
               tsmux_stream_t *p_pat,
               unsigned i_programs, tsmux_stream_t *p_pmt, const int *pi_programs_number );

typedef struct
{
    const pesmux_stream_t *pes;
    const tsmux_stream_t  *ts;
    const es_format_t  *fmt;
    int i_mapped_prog;
} pes_mapped_stream_t;

typedef enum ts_mux_standard
{
    TS_MUX_STANDARD_DVB = 0,
    TS_MUX_STANDARD_ATSC,
} ts_mux_standard;

void BuildPMT( dvbpsi_t *p_dvbpsi, vlc_object_t *p_object,
               ts_mux_standard,
               void *p_opaque, PEStoTSCallback pf_callback,
               int i_tsid, int i_pmt_version_number,
               int i_pcr_pid,
               sdt_psi_t *p_sdt,
               unsigned i_programs, tsmux_stream_t *p_pmt, const int *pi_programs_number,
               unsigned i_mapped_streams, const pes_mapped_stream_t *p_mapped_streams );


int FillPMTESParams( ts_mux_standard, const es_format_t *,
                   tsmux_stream_t *, pesmux_stream_t * );

#endif
