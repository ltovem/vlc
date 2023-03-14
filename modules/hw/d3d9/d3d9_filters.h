/*****************************************************************************
 * d3d9_filters.h : D3D9 filters module callbacks
 *****************************************************************************
 * Copyright © 2017 VLC authors, VideoLAN and VideoLabs
 *
 * Authors: Steve Lhomme <robux4@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_D3D9_FILTERS_H
#define VLC_D3D9_FILTERS_H

#include <vlc_common.h>
#include <vlc_vout_display.h>

#include "../../video_chroma/d3d9_fmt.h"

int  D3D9OpenDeinterlace(filter_t *);
int  D3D9OpenConverter(filter_t *);
int  D3D9OpenCPUConverter(filter_t *);

int D3D9OpenDecoderDevice(vlc_decoder_device *, vlc_window_t *);

#endif /* VLC_D3D9_FILTERS_H */
