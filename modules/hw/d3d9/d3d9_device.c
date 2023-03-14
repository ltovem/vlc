/*****************************************************************************
 * d3d9_device.c : D3D9 decoder device from external IDirect3DDevice9
 *****************************************************************************
 * Copyright © 2019 VLC authors, VideoLAN and VideoLabs
 *
 * Authors: Steve Lhomme <robux4@ycbcr.xyz>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_codec.h>

#include "d3d9_filters.h"

static void D3D9CloseDecoderDevice(vlc_decoder_device *device)
{
    d3d9_decoder_device_t *dec_device = device->opaque;
    D3D9_ReleaseDevice( dec_device );
}
static const struct vlc_decoder_device_operations d3d9_dev_ops = {
    .close = D3D9CloseDecoderDevice,
};

int D3D9OpenDecoderDevice(vlc_decoder_device *device, vlc_window_t *wnd)
{
    VLC_UNUSED(wnd);

    d3d9_decoder_device_t *dec_device = D3D9_CreateDevice( device );
    if ( dec_device == NULL )
        return VLC_EGENERIC;

    device->ops = &d3d9_dev_ops;
    device->opaque = dec_device;
    device->type = VLC_DECODER_DEVICE_DXVA2;
    device->sys = NULL;

    return VLC_SUCCESS;
}
