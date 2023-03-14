/*****************************************************************************
 * ts_psip_dvbpsi_fixes.h : TS demux Broken/missing dvbpsi PSIP handling
 *****************************************************************************
 * Copyright (C) 2016 - VideoLAN Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef VLC_TS_PSIP_DVBPSI_FIXES_H
#define VLC_TS_PSIP_DVBPSI_FIXES_H

dvbpsi_atsc_stt_t * DVBPlague_STT_Decode( const dvbpsi_psi_section_t* p_section );
dvbpsi_atsc_ett_t * DVBPlague_ETT_Decode( const dvbpsi_psi_section_t* p_section );

#endif
