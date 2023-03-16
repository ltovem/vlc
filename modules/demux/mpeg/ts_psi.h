// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * ts_psi.h: Transport Stream input module for VLC.
 *****************************************************************************
 * Copyright (C) 2004-2016 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef VLC_TS_PSI_H
#define VLC_TS_PSI_H

#include "ts_pid_fwd.h"

typedef enum
{
    TS_PMT_REGISTRATION_NONE = 0,
    TS_PMT_REGISTRATION_BLURAY,
    TS_PMT_REGISTRATION_ATSC,
    TS_PMT_REGISTRATION_ARIB,
} ts_pmt_registration_type_t;

bool ts_psi_PAT_Attach( ts_pid_t *, void * );
void ts_psi_Packet_Push( ts_pid_t *, const uint8_t * );

int UserPmt( demux_t *p_demux, const char * );

#endif
