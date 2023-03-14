/*****************************************************************************
 * d3d11_filters.h : D3D11 filters module callbacks
 *****************************************************************************
 * Copyright © 2017 VLC authors, VideoLAN and VideoLabs
 *
 * Authors: Steve Lhomme <robux4@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_D3D11_FILTERS_H
#define VLC_D3D11_FILTERS_H

#include <vlc_common.h>
#include <vlc_vout_display.h>

#include "../../video_chroma/d3d11_fmt.h"

#ifdef __cplusplus
extern "C" {
#endif

int  D3D11OpenDeinterlace(filter_t *);
int  D3D11OpenConverter(filter_t *);
int  D3D11OpenCPUConverter(filter_t *);
int  D3D11OpenBlockDecoder(vlc_object_t *);
void D3D11CloseBlockDecoder(vlc_object_t *);

int  D3D11OpenDecoderDeviceW8(vlc_decoder_device *, vlc_window_t *);
int  D3D11OpenDecoderDeviceAny(vlc_decoder_device *, vlc_window_t *);

#ifdef __cplusplus
}
#endif

#endif /* VLC_D3D11_FILTERS_H */
