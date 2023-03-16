// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * dxgi_swapchain.h: DXGI swapchain handled by the display module
 *****************************************************************************
 * Copyright (C) 2014-2021 VLC authors and VideoLAN
 *
 * Authors: Martell Malone <martellmalone@gmail.com>
 *          Steve Lhomme <robux4@gmail.com>
 *****************************************************************************/

#ifndef VLC_DXGI_SWAPCHAIN_H
#define VLC_DXGI_SWAPCHAIN_H

#include <vlc_common.h>

#include <vlc/libvlc.h>
#include <vlc/libvlc_picture.h>
#include <vlc/libvlc_media.h>
#include <vlc/libvlc_renderer_discoverer.h>
#include <vlc/libvlc_media_player.h>

#include <wrl/client.h>

#include <dxgi1_5.h>
#include "../../video_chroma/dxgi_fmt.h"


#ifndef IID_GRAPHICS_PPV_ARGS
#define IID_GRAPHICS_PPV_ARGS(ppType) IID_PPV_ARGS(ppType)
#endif

#define DXGI_SWAP_FRAME_COUNT   3

struct dxgi_swapchain;

struct dxgi_swapchain *DXGI_CreateLocalSwapchainHandleHwnd(vlc_object_t *, HWND);

#if defined(HAVE_DCOMP_H) && !defined(VLC_WINSTORE_APP)
struct dxgi_swapchain *DXGI_CreateLocalSwapchainHandleDComp(vlc_object_t *,
                                           void /*IDCompositionDevice*/ * dcompDevice,
                                           void /*IDCompositionVisual*/ * dcompVisual);
#endif

Microsoft::WRL::ComPtr<IDXGISwapChain1> & DXGI_GetSwapChain1( struct dxgi_swapchain * );
Microsoft::WRL::ComPtr<IDXGISwapChain4> & DXGI_GetSwapChain4( struct dxgi_swapchain * );
const d3d_format_t  *DXGI_GetPixelFormat( struct dxgi_swapchain * );

void DXGI_SelectSwapchainColorspace( struct dxgi_swapchain *, const libvlc_video_render_cfg_t * );
void DXGI_LocalSwapchainCleanupDevice( struct dxgi_swapchain * );
void DXGI_SwapchainUpdateOutput( struct dxgi_swapchain *, libvlc_video_output_cfg_t * );
bool DXGI_UpdateSwapChain( struct dxgi_swapchain *, IDXGIAdapter *,
                           IUnknown *pFactoryDevice,
                           const d3d_format_t *, const libvlc_video_render_cfg_t * );

void DXGI_LocalSwapchainSwap( struct dxgi_swapchain * );
void DXGI_LocalSwapchainSetMetadata( struct dxgi_swapchain *, libvlc_video_metadata_type_t, const void * );

#endif /* VLC_DXGI_SWAPCHAIN_H */
