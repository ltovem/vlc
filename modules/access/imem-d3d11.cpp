/*****************************************************************************
 * imem-d3d11.cpp : D3D11 Texture input for VLC
 *****************************************************************************
 * Copyright © 2023 VideoLabs, VLC authors and VideoLAN
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Authors: Steve Lhomme <robux4@videolabs.io>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_demux.h>
#include <vlc_input_item.h>

#include <vlc/libvlc.h>
#include <vlc/libvlc_picture.h>
#include <vlc/libvlc_media.h>
#include <vlc/libvlc_renderer_discoverer.h>
#include <vlc/libvlc_media_player.h>

#include "../video_chroma/d3d11_fmt.h"

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

static int  OpenDemux (vlc_object_t *);
static void CloseDemux(vlc_object_t *);

vlc_module_begin()
    set_shortname(N_("Memory input"))
    set_description(N_("Memory input"))
    set_subcategory(SUBCAT_INPUT_ACCESS)
    add_shortcut("imem")
    set_capability("access", 100)
    set_callbacks(OpenDemux, CloseDemux)
vlc_module_end()

struct imem_dxgi_t
{
    void *opaque = nullptr;
    libvlc_media_getbuf_cb get_cb = nullptr;
    libvlc_media_releasebuf_cb release_cb = nullptr;
    libvlc_media_close_cb close_cb = nullptr;

    es_out_id_t *es = nullptr;
    es_format_t fmt;
    vlc_tick_t pts = VLC_TICK_INVALID;

    // TODO decoder device for the external textures
    const d3d_format_t *output_format = nullptr;
    d3d11_device_t *d3ddev = nullptr;
    vlc_video_context *vctx = nullptr;
};

static void CaptureBlockRelease( block_t *p_block )
{
    block_sys_d3d11_t *d3d11_block = D3D11BLOCK_FROM_BLOCK(p_block);
    picture_Release(d3d11_block->d3d11_pic);
    delete d3d11_block;
}

static bool SetupDevice(void **opaque, const libvlc_video_setup_device_cfg_t *,
                        libvlc_video_setup_device_info_t *out)
{
    ComPtr<ID3D11Texture2D> tex = static_cast<ID3D11Texture2D*>(*opaque);
    ComPtr<ID3D11Device> d3d;
    tex->GetDevice(d3d.GetAddressOf());
    ComPtr<ID3D11DeviceContext> d3dctx;
    d3d->GetImmediateContext(d3dctx.GetAddressOf());
    // TODO keep a reference and release later

    out->d3d11.device_context = d3dctx.Get();
    out->d3d11.context_mutex = nullptr;

    return true;
}

static int Demux(demux_t *p_demux)
{
    int res = VLC_DEMUXER_EGENERIC;
    auto sys = static_cast<imem_dxgi_t*>(p_demux->p_sys);
    d3d11_decoder_device_t *dev_sys = nullptr;
    picture_sys_d3d11_t *picsys = nullptr;
    ComPtr<ID3D11Resource> d3dres;
    ComPtr<ID3D11Texture2D> tex;

    block_sys_d3d11_t *d3d11_block = new (std::nothrow) block_sys_d3d11_t();
    if (unlikely(d3d11_block == nullptr))
        return 0;

    void *ptr_tex = nullptr;
    int64_t pts_us;
    auto ptr = sys->get_cb(sys->opaque, &ptr_tex, &pts_us);
    if (ptr < 0 || ptr_tex == nullptr)
    {
        goto error;
    }
    if (ptr == 0)
    {
        res = VLC_DEMUXER_EOF;
        goto error;
    }
    d3dres = static_cast<ID3D11Resource *>(ptr_tex);
    if (FAILED(d3dres.As(&tex)))
    {
        goto error;
    }

    if (sys->es == nullptr)
    {
        D3D11_TEXTURE2D_DESC desc;
        tex->GetDesc(&desc);
        for (const d3d_format_t *output_format = DxgiGetRenderFormatList();
                output_format->name != NULL; ++output_format)
        {
            if (output_format->formatTexture == desc.Format &&
                is_d3d11_opaque(output_format->fourcc))
            {
                sys->output_format = output_format;
                break;
            }
        }

        if (sys->output_format == nullptr)
        {
            msg_Err(p_demux, "Unknown texture format %d", desc.Format);
            goto error;
        }

        // TODO this could be set by the libvlc API
        var_Create(p_demux, "dec-dev", VLC_VAR_STRING);
        var_SetString(p_demux, "dec-dev", "d3d11");
        var_Create(p_demux, "vout-cb-type", VLC_VAR_INTEGER);
        var_SetInteger(p_demux, "vout-cb-type", libvlc_video_engine_d3d11);

        var_Create(p_demux, "vout-cb-setup", VLC_VAR_ADDRESS);
        var_Create(p_demux, "vout-cb-opaque", VLC_VAR_ADDRESS);

        libvlc_video_output_setup_cb setup_cb = SetupDevice;
        var_SetAddress(p_demux, "vout-cb-setup", (void*)setup_cb);
        var_SetAddress(p_demux, "vout-cb-opaque", tex.Get());

        auto dec_dev = vlc_decoder_device_Create(VLC_OBJECT(p_demux), nullptr);
        if (dec_dev == nullptr)
        {
            msg_Err(p_demux, "Failed to use the external");
            goto error;
        }
        sys->vctx = D3D11CreateVideoContext(dec_dev, sys->output_format->formatTexture);
        vlc_decoder_device_Release(dec_dev);
        dec_dev = nullptr;
        if (unlikely(sys->vctx == nullptr))
        {
            msg_Err(p_demux, "Failed to create the video context");
            goto error;
        }

        es_format_Init(&sys->fmt, VIDEO_ES, sys->output_format->fourcc);

        sys->fmt.video.i_visible_width    =
        sys->fmt.video.i_width            = desc.Width;
        sys->fmt.video.i_visible_height   =
        sys->fmt.video.i_height           = desc.Height;
        sys->fmt.video.i_bits_per_pixel   = 4 * sys->output_format->bitsPerChannel; /* FIXME */
        sys->fmt.video.i_sar_num = sys->fmt.video.i_sar_den = 1;
        sys->fmt.video.i_chroma           = sys->fmt.i_codec;
        sys->fmt.video.color_range        = COLOR_RANGE_FULL; // FIXME it may be something else
        // sys->fmt.video.i_frame_rate       = desc.ModeDesc.RefreshRate.Numerator;
        // sys->fmt.video.i_frame_rate_base  = desc.ModeDesc.RefreshRate.Denominator;

        sys->es = es_out_Add(p_demux->out, &sys->fmt);
        if (sys->es == nullptr)
        {
            goto error;
        }
    }

    static const struct vlc_frame_callbacks cbs = {
        CaptureBlockRelease,
    };
    block_Init( &d3d11_block->self, &cbs, nullptr, 1 );

    // TODO avoid a copy if the original has D3D11_RESOURCE_MISC_SHARED_NTHANDLE
    d3d11_block->d3d11_pic = D3D11_AllocPicture(VLC_OBJECT(p_demux),
                                                &sys->fmt.video, sys->vctx,
                                                true, sys->output_format);
    if ( d3d11_block->d3d11_pic == nullptr )
    {
        msg_Err(p_demux, "Failed to allocate the output texture");
        goto error;
    }

    picsys = ActiveD3D11PictureSys(d3d11_block->d3d11_pic);
    dev_sys = GetD3D11OpaqueContext(sys->vctx);
    dev_sys->d3d_dev.d3dcontext->CopyResource(picsys->texture[0], tex.Get());

    sys->release_cb(sys->opaque, ptr_tex);

    sys->pts = VLC_TICK_FROM_US(pts_us) + VLC_TICK_0;
    d3d11_block->self.i_pts = sys->pts;

    es_out_SetPCR(p_demux->out, sys->pts);
    es_out_Send(p_demux->out, sys->es, &d3d11_block->self);

    return VLC_DEMUXER_SUCCESS;
error:
    if (ptr_tex != nullptr)
        sys->release_cb(sys->opaque, ptr_tex);
    if (d3d11_block->d3d11_pic)
        picture_Release(d3d11_block->d3d11_pic);
    delete d3d11_block;
    return res;
}

static int Control(demux_t *p_demux, int i_query, va_list ap)
{
    auto sys = static_cast<imem_dxgi_t*>(p_demux->p_sys);
    switch (i_query)
    {
        case DEMUX_CAN_RECORD:
        case DEMUX_CAN_CONTROL_PACE:
            *va_arg(ap, bool *) = true;
            return VLC_SUCCESS;
        case DEMUX_CAN_PAUSE: // TODO legacy imem can be "paused"
        case DEMUX_CAN_SEEK:
            *va_arg(ap, bool *) = false;
            return VLC_SUCCESS;
        case DEMUX_GET_PTS_DELAY:
            *va_arg(ap, vlc_tick_t *) = DEFAULT_PTS_DELAY; // TODO should be 0 ?
            return VLC_SUCCESS;
        case DEMUX_GET_TYPE:
            *va_arg(ap, int *) = ITEM_TYPE_STREAM;
            return VLC_SUCCESS;
        case DEMUX_GET_LENGTH:
            *va_arg(ap, vlc_tick_t *) = 0;
            return VLC_SUCCESS;
        case DEMUX_TEST_AND_CLEAR_FLAGS:
            *va_arg(ap, unsigned *) = 0;
            return VLC_SUCCESS;
        case DEMUX_GET_TIME:
            *va_arg(ap, vlc_tick_t *) = sys->pts;
            return VLC_SUCCESS;

        // TODO get it from the source if it's constant
        case DEMUX_GET_FPS:
            return VLC_EGENERIC;

        // knowingly unsupported
        case DEMUX_GET_POSITION:
        case DEMUX_GET_TITLE_INFO:
        case DEMUX_GET_ATTACHMENTS:
        case DEMUX_SET_GROUP_DEFAULT:
        case DEMUX_GET_META:
        case DEMUX_HAS_UNSUPPORTED_META:
        case DEMUX_GET_SIGNAL:
        case DEMUX_GET_NORMAL_TIME:
            return VLC_EGENERIC;
    }
    return VLC_EGENERIC;
}


int OpenDemux(vlc_object_t *obj)
{
    demux_t *p_demux = (demux_t *)obj;

    if (p_demux->out == nullptr)
        return VLC_EGENERIC;

    if (var_InheritInteger(p_demux, "imem-type") != libvlc_media_source_d3d11)
        return VLC_ENOTSUP;

    imem_dxgi_t *sys = new (std::nothrow) imem_dxgi_t();
    if (unlikely(sys == nullptr))
        return VLC_ENOMEM;

    libvlc_media_open_cb open_cb;
    void *opaque;

    opaque = var_InheritAddress(p_demux, "imem-data");
    open_cb = (libvlc_media_open_cb)var_InheritAddress(p_demux, "imem-open");
    assert(open_cb != nullptr);

    sys->get_cb = (libvlc_media_getbuf_cb)var_InheritAddress(p_demux, "imem-read");
    sys->release_cb = (libvlc_media_releasebuf_cb)var_InheritAddress(p_demux, "imem-release");
    if (sys->get_cb == nullptr || sys->release_cb == nullptr)
    {
        msg_Err(p_demux, "missing get/release callbacks");
        goto error;
    }

    sys->close_cb = (libvlc_media_close_cb)var_InheritAddress(p_demux, "imem-close");

    uint64_t dummy;
    if (open_cb(opaque, &sys->opaque, &dummy)) {
        msg_Err(p_demux, "open error");
        goto error;
    }

    p_demux->pf_read = nullptr;
    p_demux->pf_block = nullptr;
    p_demux->pf_seek = nullptr;
    p_demux->pf_control = Control;
    p_demux->pf_demux = Demux;

    p_demux->p_sys = sys;
    return VLC_SUCCESS;
error:
    CloseDemux(obj);
    return VLC_EGENERIC;
}

void CloseDemux(vlc_object_t *obj)
{
    demux_t *p_demux = (demux_t *)obj;
    imem_dxgi_t *sys = static_cast<imem_dxgi_t *>(p_demux->p_sys);

    if (sys->close_cb)
        sys->close_cb(sys->opaque);

    if (sys->es != nullptr)
        es_out_Del(p_demux->out, sys->es);
   delete sys;
}