/*****************************************************************************
 * d3d11_swapchain.h: Direct3D11 swapchain handled by the display module
 *****************************************************************************
 * Copyright (C) 2014-2019 VLC authors and VideoLAN
 *
 * Authors: Martell Malone <martellmalone@gmail.com>
 *          Steve Lhomme <robux4@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_D3D11_SWAPCHAIN_H
#define VLC_D3D11_SWAPCHAIN_H

#include <vlc_common.h>
#include "dxgi_swapchain.h"
#include "../../video_chroma/d3d11_fmt.h"

void *D3D11_CreateLocalSwapchainHandleHwnd(vlc_object_t *, HWND, d3d11_device_t *d3d_dev);
#if defined(HAVE_DCOMP_H) && !defined(VLC_WINSTORE_APP)
void *D3D11_CreateLocalSwapchainHandleDComp(vlc_object_t *, void* dcompDevice, void* dcompVisual, d3d11_device_t *d3d_dev);
#endif

void D3D11_LocalSwapchainCleanupDevice( void *opaque );
bool D3D11_LocalSwapchainUpdateOutput( void *opaque, const libvlc_video_render_cfg_t *cfg, libvlc_video_output_cfg_t *out );
bool D3D11_LocalSwapchainStartEndRendering( void *opaque, bool enter );
bool D3D11_LocalSwapchainSelectPlane( void *opaque, size_t plane, void *output );
void D3D11_LocalSwapchainSwap( void *opaque );
void D3D11_LocalSwapchainSetMetadata( void *opaque, libvlc_video_metadata_type_t, const void * );

#endif /* VLC_D3D11_SWAPCHAIN_H */
