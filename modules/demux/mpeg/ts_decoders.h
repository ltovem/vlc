// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * ts_decoders.h: TS Demux custom tables decoders
 *****************************************************************************
 * Copyright (C) 2016 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef VLC_TS_DECODERS_H
#define VLC_TS_DECODERS_H

typedef void (* ts_dvbpsi_rawsections_callback_t)( dvbpsi_t *p_dvbpsi,
                                                   const dvbpsi_psi_section_t* p_section,
                                                   void* p_cb_data );

bool ts_dvbpsi_AttachRawSubDecoder( dvbpsi_t* p_dvbpsi,
                                    uint8_t i_table_id, uint16_t i_extension,
                                    ts_dvbpsi_rawsections_callback_t pf_callback,
                                    void* p_cb_data );

void ts_dvbpsi_DetachRawSubDecoder( dvbpsi_t *p_dvbpsi, uint8_t i_table_id, uint16_t i_extension );

bool ts_dvbpsi_AttachRawDecoder( dvbpsi_t* p_dvbpsi,
                                 ts_dvbpsi_rawsections_callback_t pf_callback,
                                 void* p_cb_data );

void ts_dvbpsi_DetachRawDecoder( dvbpsi_t *p_dvbpsi );

#endif
