/*****************************************************************************
 * d3d11_shaders.h: Direct3D11 Shaders
 *****************************************************************************
 * Copyright (C) 2017-2021 VLC authors and VideoLAN
 *
 * Authors: Steve Lhomme <robux4@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_D3D11_SHADERS_H
#define VLC_D3D11_SHADERS_H

#include "d3d_shaders.h"
#include "d3d_dynamic_shader.h"

#include "../../video_chroma/d3d11_fmt.h"

#include <wrl/client.h>

/* Vertex Shader compiled structures */
struct d3d11_vertex_shader_t {
    Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>  layout;
};

/* A Quad is texture that can be displayed in a rectangle */
struct d3d11_quad_t
{
    ~d3d11_quad_t()
    {
        ReleaseD3D11PictureSys(&picSys);
    }

    void Reset();
    void UpdateViewport(const RECT *, const d3d_format_t *display);

    picture_sys_d3d11_t       picSys = {};
    d3d_quad_t                generic = {};
    UINT                      resourceCount = 0;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> viewpointShaderConstant;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pPixelShaderConstants;
    UINT                       PSConstantsCount = 0;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>  d3dpixelShader[DXGI_MAX_RENDER_TARGET];
    Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerStates[2];
    D3D11_VIEWPORT            cropViewport[DXGI_MAX_RENDER_TARGET];

    PS_CONSTANT_BUFFER        pConstants;
    VS_PROJECTION_CONST       vConstants;
};

HRESULT D3D11_CompilePixelShaderBlob(vlc_object_t *, const d3d_shader_compiler_t *,
                                 d3d11_device_t *, const display_info_t *,
                                 video_transfer_func_t,
                                 bool src_full_range,
                                 const d3d11_quad_t *, d3d_shader_blob pPSBlob[DXGI_MAX_RENDER_TARGET]);
#define D3D11_CompilePixelShaderBlob(a,b,c,d,e,f,g,h) \
    D3D11_CompilePixelShaderBlob(VLC_OBJECT(a),b,c,d,e,f,g,h)
HRESULT D3D11_SetQuadPixelShader(vlc_object_t *, d3d11_device_t *,
                                 bool sharp,
                                 d3d11_quad_t *quad, d3d_shader_blob pPSBlob[DXGI_MAX_RENDER_TARGET]);

HRESULT D3D11_CompileVertexShaderBlob(vlc_object_t *, const d3d_shader_compiler_t *,
                                      d3d11_device_t *, bool flat, d3d_shader_blob *);

HRESULT D3D11_CreateVertexShader(vlc_object_t *, d3d_shader_blob *, d3d11_device_t *, d3d11_vertex_shader_t *);
#define D3D11_CreateVertexShader(a,b,c,d) D3D11_CreateVertexShader(VLC_OBJECT(a),b,c,d)

HRESULT D3D11_CreateRenderTargets(d3d11_device_t *, ID3D11Resource *, const d3d_format_t *,
                                  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> output[DXGI_MAX_RENDER_TARGET]);

void D3D11_ClearRenderTargets(d3d11_device_t *, const d3d_format_t *,
                              Microsoft::WRL::ComPtr<ID3D11RenderTargetView> targets[DXGI_MAX_RENDER_TARGET]);

void D3D11_ReleaseVertexShader(d3d11_vertex_shader_t *);

#endif /* VLC_D3D11_SHADERS_H */
