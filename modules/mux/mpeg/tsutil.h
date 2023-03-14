/*****************************************************************************
 * tsutil.h
 *****************************************************************************
 * Copyright (C) 2001-2005, 2015 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef VLC_MPEG_TSUTIL_H_
#define VLC_MPEG_TSUTIL_H_

typedef void(*PEStoTSCallback)(void *, block_t *);

void PEStoTS( void *p_opaque, PEStoTSCallback pf_callback, block_t *p_pes,
              uint16_t i_pid, bool *pb_discontinuity, uint8_t *pi_continuity_counter );

#endif
