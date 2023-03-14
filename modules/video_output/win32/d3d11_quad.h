/*****************************************************************************
 * d3d11_quad.h: Direct3D11 Quad handling
 *****************************************************************************
 * Copyright (C) 2017-2018 VLC authors and VideoLAN
 *
 * Authors: Steve Lhomme <robux4@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_D3D11_QUAD_H
#define VLC_D3D11_QUAD_H

#include "../../video_chroma/d3d11_fmt.h"
#include "d3d11_shaders.h"

#define PS_CONST_LUMI_BOUNDS 0
#define VS_CONST_VIEWPOINT   1

typedef bool (*d3d11_select_plane_t)(void *opaque, size_t plane_index, ID3D11RenderTargetView **);

void D3D11_RenderQuad(d3d11_device_t *, d3d11_quad_t *, d3d11_vertex_shader_t *,
                      ID3D11ShaderResourceView *resourceViews[DXGI_MAX_SHADER_VIEW],
                      d3d11_select_plane_t selectPlane, void *selectOpaque);

int D3D11_AllocateQuad(vlc_object_t *, d3d11_device_t *, video_projection_mode_t, d3d11_quad_t *);
#define D3D11_AllocateQuad(a,b,c,d)  D3D11_AllocateQuad(VLC_OBJECT(a),b,c,d)

int D3D11_SetupQuad(vlc_object_t *, d3d11_device_t *, const video_format_t *, d3d11_quad_t *,
                    const display_info_t *);
#define D3D11_SetupQuad(a,b,c,d,e)  D3D11_SetupQuad(VLC_OBJECT(a),b,c,d,e)

bool D3D11_UpdateQuadPosition( vlc_object_t *, d3d11_device_t *, d3d11_quad_t *,
                               const RECT *output, video_transform_t );
#define D3D11_UpdateQuadPosition(a,b,c,d,e)  D3D11_UpdateQuadPosition(VLC_OBJECT(a),b,c,d,e)

void D3D11_UpdateQuadOpacity(vlc_object_t *, d3d11_device_t *, d3d11_quad_t *, float opacity);
#define D3D11_UpdateQuadOpacity(a,b,c,d)  D3D11_UpdateQuadOpacity(VLC_OBJECT(a),b,c,d)

void D3D11_UpdateQuadLuminanceScale(vlc_object_t *, d3d11_device_t *, d3d11_quad_t *, float luminanceScale);
#define D3D11_UpdateQuadLuminanceScale(a,b,c,d)  D3D11_UpdateQuadLuminanceScale(VLC_OBJECT(a),b,c,d)

void D3D11_UpdateViewpoint(vlc_object_t *, d3d11_device_t *, d3d11_quad_t *, const vlc_viewpoint_t*, float sar);
#define D3D11_UpdateViewpoint(a,b,c,d,e)  D3D11_UpdateViewpoint(VLC_OBJECT(a),b,c,d,e)

#endif /* VLC_D3D11_QUAD_H */
