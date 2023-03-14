/*****************************************************************************
 * repack.h: Codec specific formatting for AnnexB multiplexers
 *****************************************************************************
 * Copyright (C) 2011 VideoLabs, VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

block_t * PES_Repack(vlc_fourcc_t i_codec,
                     const uint8_t *p_extra, size_t i_extra,
                     block_t **pp_pes);
