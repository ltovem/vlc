// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * ts_scte.h: TS Demux SCTE section decoders/handlers
 *****************************************************************************
 * Copyright (C) 2004-2016 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef VLC_TS_SCTE_H
#define VLC_TS_SCTE_H

#include "../../codec/scte18.h"

void SCTE18_Section_Callback( dvbpsi_t *p_handle,
                              const dvbpsi_psi_section_t* p_section,
                              void *p_cb_data );
void SCTE27_Section_Callback( demux_t *p_demux,
                              const uint8_t *, size_t,
                              const uint8_t *, size_t,
                              void * );

#endif
