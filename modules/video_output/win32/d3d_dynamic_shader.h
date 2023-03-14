/*****************************************************************************
 * d3d_dynamic_shader.h: Direct3D Shader Blob generation
 *****************************************************************************
 * Copyright (C) 2017-2021 VLC authors and VideoLAN
 *
 * Authors: Steve Lhomme <robux4@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_D3D_DYNAMIC_SHADER_H
#define VLC_D3D_DYNAMIC_SHADER_H

#include <d3dcommon.h>

#ifdef __cplusplus
extern "C" {
#endif// __cplusplus

typedef struct d3d_shader_compiler_t d3d_shader_compiler_t;

int D3D_CreateShaderCompiler(vlc_object_t *, d3d_shader_compiler_t **);
void D3D_ReleaseShaderCompiler(d3d_shader_compiler_t *);

HRESULT D3D_CompilePixelShader(vlc_object_t *, const d3d_shader_compiler_t *,
                               D3D_FEATURE_LEVEL,
                               const display_info_t *,
                               video_transfer_func_t,
                               bool src_full_range,
                               const d3d_format_t *dxgi_fmt,
                               d3d_shader_blob pPSBlob[DXGI_MAX_RENDER_TARGET],
                               size_t shader_views[DXGI_MAX_RENDER_TARGET]);

HRESULT D3D_CompileVertexShader(vlc_object_t *, const d3d_shader_compiler_t *,
                                D3D_FEATURE_LEVEL, bool flat,
                                d3d_shader_blob *);

#ifdef __cplusplus
}
#endif// __cplusplus

#endif /* VLC_D3D_DYNAMIC_SHADER_H */
