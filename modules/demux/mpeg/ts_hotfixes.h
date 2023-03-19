// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * ts_hotfixes.h : MPEG PMT/PAT less streams fixups
 *****************************************************************************
 * Copyright (C) 2014-2016 - VideoLAN Authors
 *****************************************************************************/
#ifndef VLC_TS_HOTFIXES_H
#define VLC_TS_HOTFIXES_H

#define MIN_PAT_INTERVAL VLC_TICK_FROM_SEC(1) // DVB is 500ms

void ProbePES( demux_t *p_demux, ts_pid_t *pid, const uint8_t *p_pesstart, size_t i_data, bool b_adaptfield );
void MissingPATPMTFixup( demux_t *p_demux );

#endif
