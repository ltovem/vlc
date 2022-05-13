/*****************************************************************************
 * avfilter.c : avfilter audio emphasis filter
 *****************************************************************************
 * Copyright © 2022 VLC authors, VideoLAN and VideoLabs
 *
 * Authors: Steve Lhomme <robux4@videolabs.io>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_aout.h>
#include <vlc_filter.h>

#include "avfilter.h"

#include <libavfilter/avfilter.h>
#include <libavfilter/buffersrc.h>
#include <libavfilter/buffersink.h>
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/

#ifndef MERGE_FFMPEG
vlc_module_begin()
    AVEMPHASIS_MODULE
vlc_module_end()
#endif

typedef struct lavfa_sys
{
    AVFilterGraph *filter_graph;
    AVFilterContext *buffersrc_ctx;
    AVFilterContext *buffersink_ctx;
    enum AVSampleFormat format;
    int64_t channel_layout;
} lavfa_sys;

/**
 * Allocates decoded audio buffer for libavcodec to use.
 */
typedef struct
{
    block_t self;
    AVFrame *frame;
} vlc_av_frame_t;

static void vlc_av_frame_Release(block_t *block)
{
    vlc_av_frame_t *b = (void *)block;

    av_frame_free(&b->frame);
    free(b);
}

static const struct vlc_block_callbacks vlc_av_frame_cbs =
{
    vlc_av_frame_Release,
};

static block_t *vlc_av_frame_Wrap(AVFrame *frame)
{
    for (unsigned i = 1; i < AV_NUM_DATA_POINTERS; i++)
        assert(frame->linesize[i] == 0); /* only packed frame supported */

    if (av_frame_make_writable(frame)) /* TODO: read-only block_t */
        return NULL;

    vlc_av_frame_t *b = malloc(sizeof (*b));
    if (unlikely(b == NULL))
        return NULL;

    block_t *block = &b->self;

    block_Init(block, &vlc_av_frame_cbs,
               frame->extended_data[0], frame->linesize[0]);
    block->i_nb_samples = frame->nb_samples;
    b->frame = frame;
    return block;
}

/*****************************************************************************
 * DoWork: convert a buffer
 *****************************************************************************/
static block_t *DoWork( filter_t * p_filter, block_t * p_in_buf )
{
    lavfa_sys *sys = p_filter->p_sys;
    int ret;
    AVFrame *srcFrame = av_frame_alloc();
    if (unlikely(!srcFrame))
    {
        block_Release(p_in_buf);
        return NULL;
    }
    srcFrame->data[0] = p_in_buf->p_buffer;
    srcFrame->linesize[0] = p_in_buf->i_buffer;
    srcFrame->format = sys->format;
    srcFrame->nb_samples = p_in_buf->i_nb_samples;
    srcFrame->sample_rate = p_filter->fmt_in.audio.i_rate;
    srcFrame->channel_layout = sys->channel_layout;
    srcFrame->channels = p_filter->fmt_in.audio.i_channels;

    ret = av_buffersrc_add_frame(sys->buffersrc_ctx, srcFrame);
    block_Release(p_in_buf);
    av_frame_unref(srcFrame);
    if (ret < 0)
    {
        return NULL;
    }

    AVFrame *dstFrame = av_frame_alloc();
    if (unlikely(!dstFrame))
    {
        return NULL;
    }

    ret = av_buffersink_get_frame(sys->buffersink_ctx, dstFrame);
    if (ret < 0)
    {
        av_frame_unref(dstFrame);
        return NULL;
    }

    return vlc_av_frame_Wrap(dstFrame);
}

static int VlcToAvFilterEmphasis(audio_emphasis_type_t emphasis)
{
    switch (emphasis)
    {
        case AUDIO_EMPHASIS_CD_50_DIV_15_uS: return 4;
        case AUDIO_EMPHASIS_PHONO_RIAA:      return 3;
        case AUDIO_EMPHASIS_FM_50_uS:        return 5;
        case AUDIO_EMPHASIS_FM_75_uS:        return 6;
        case AUDIO_EMPHASIS_SAT_50_uS:       return 7;
        case AUDIO_EMPHASIS_SAT_75_uS:       return 8;
        case AUDIO_EMPHASIS_NONE:            return -1;
        case AUDIO_EMPHASIS_CCITT_J17:
        default:
            return INT_MAX; // not supported
    }
}

static void Close(filter_t *p_filter)
{
    lavfa_sys *sys = p_filter->p_sys;
    avfilter_graph_free(&sys->filter_graph);
}

static enum AVSampleFormat GetLavAudioFormat( vlc_fourcc_t fmt )
{
    switch (fmt)
    {
        case VLC_CODEC_U8:   return AV_SAMPLE_FMT_U8;
        case VLC_CODEC_S16N: return AV_SAMPLE_FMT_S16;
        case VLC_CODEC_S32N: return AV_SAMPLE_FMT_S32;
        case VLC_CODEC_FL32: return AV_SAMPLE_FMT_FLT;
        case VLC_CODEC_FL64: return AV_SAMPLE_FMT_DBL;
        default:
            return  AV_SAMPLE_FMT_NONE;
    }
}


/*****************************************************************************
 * avemphasis_Create: allocate filter
 *****************************************************************************/
int avemphasis_Create( vlc_object_t *p_this )
{
    filter_t * p_filter = (filter_t *)p_this;
    const audio_format_t *infmt = &p_filter->fmt_in.audio;
    const audio_format_t *outfmt = &p_filter->fmt_out.audio;

    if( infmt->emphasis == outfmt->emphasis )
        return VLC_EGENERIC;

    if( infmt->i_format != outfmt->i_format ||
        infmt->i_channels != outfmt->i_channels ||
        !AOUT_FMT_LINEAR( infmt ) )
        return VLC_ENOTSUP;

    if( VlcToAvFilterEmphasis(infmt->emphasis) == INT_MAX ||
        VlcToAvFilterEmphasis(outfmt->emphasis) == INT_MAX )
        // not supported in libavfilter
        return VLC_EGENERIC;

    enum AVSampleFormat lav_fmt = GetLavAudioFormat(infmt->i_format);
    if (unlikely(lav_fmt == AV_SAMPLE_FMT_NONE))
        return VLC_ENOTSUP;

    const AVFilter *avemph = avfilter_get_by_name("aemphasis");
    const AVFilter *aformat = avfilter_get_by_name("aformat");
    const AVFilter *abuffersrc  = avfilter_get_by_name("abuffer");
    const AVFilter *abuffersink = avfilter_get_by_name("abuffersink");
    if (unlikely(!avemph || !abuffersrc || !abuffersink))
        return VLC_ENOTSUP;

    lavfa_sys *sys = vlc_obj_malloc(p_this, sizeof(lavfa_sys));
    if (unlikely(!sys))
    {
        return VLC_ENOMEM;
    }

    sys->format = lav_fmt;
    sys->filter_graph = avfilter_graph_alloc();
    if (unlikely(!sys->filter_graph))
    {
        return VLC_ENOMEM;
    }

    int ret;
    char args[512];
    sys->channel_layout = av_get_default_channel_layout( infmt->i_channels );
    enum AVSampleFormat out_sample_fmts[] = { sys->format, AV_SAMPLE_FMT_NONE };
    int64_t out_channel_layouts[] = { sys->channel_layout, -1 };
    int out_sample_rates[] = { infmt->i_rate, -1 };

    ret = snprintf(args, sizeof(args),
                   "sample_rate=%d:sample_fmt=%s:channel_layout=0x%"PRIx64,
                   infmt->i_rate, av_get_sample_fmt_name(sys->format),
                   sys->channel_layout);
    assert(ret < (int)sizeof(args));
    ret = avfilter_graph_create_filter(&sys->buffersrc_ctx, abuffersrc, "in",
                                       args, NULL, sys->filter_graph);
    if (ret < 0) {
        msg_Err(p_this, "Cannot set output sample format\n");
        goto end;
    }

    ret = avfilter_graph_create_filter(&sys->buffersink_ctx, abuffersink, "out",
                                       NULL, NULL, sys->filter_graph);
    if (ret < 0) {
        msg_Err(p_this, "Cannot set output sample format\n");
        goto end;
    }

    ret = av_opt_set_int_list(sys->buffersink_ctx, "sample_fmts",
                              out_sample_fmts, -1, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        msg_Err(p_this, "Cannot set output sample format\n");
        goto end;
    }

    ret = av_opt_set_int_list(sys->buffersink_ctx, "channel_layouts",
                              out_channel_layouts, -1, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        msg_Err(p_this, "Cannot set output channel layout\n");
        goto end;
    }

    ret = av_opt_set_int_list(sys->buffersink_ctx, "sample_rates",
                              out_sample_rates, -1, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        msg_Err(p_this, "Cannot set output sample rate\n");
        goto end;
    }

    char text_filter_str[512] = {0};
    AVFilterContext *ctx_in = sys->buffersrc_ctx;
    if (infmt->emphasis != AUDIO_EMPHASIS_NONE)
    {
        ret  = snprintf(text_filter_str, sizeof(text_filter_str),
                        "type=%d:mode=reproduction", VlcToAvFilterEmphasis(infmt->emphasis));
        assert(ret < (int)sizeof(text_filter_str));

        AVFilterContext* emph_in_ctx;
        emph_in_ctx = avfilter_graph_alloc_filter(sys->filter_graph, avemph, "aemphasis_in");
        if ((ret = avfilter_init_str(emph_in_ctx, text_filter_str)) < 0)
            goto end;

        if ((ret = avfilter_link(ctx_in, 0, emph_in_ctx, 0)) < 0)
            goto end;
        ctx_in = emph_in_ctx;
    }

    if (outfmt->emphasis != AUDIO_EMPHASIS_NONE)
    {
        ret = snprintf(text_filter_str, sizeof(text_filter_str),
                       "type=%d:mode=production",VlcToAvFilterEmphasis(outfmt->emphasis));
        assert(ret < (int)sizeof(text_filter_str));

        AVFilterContext* emph_out_ctx;
        emph_out_ctx = avfilter_graph_alloc_filter(sys->filter_graph, avemph, "aemphasis_out");
        if ((ret = avfilter_init_str(emph_out_ctx, text_filter_str)) < 0)
            goto end;

        if ((ret = avfilter_link(ctx_in, 0, emph_out_ctx, 0)) < 0)
            goto end;
        ctx_in = emph_out_ctx;
    }

    // convert back to what we started with
    char ch_layout[64];
    av_get_channel_layout_string(ch_layout, sizeof(ch_layout), 0, sys->channel_layout);
    ret = snprintf(text_filter_str, sizeof(text_filter_str),
                   "sample_fmts=%s:channel_layouts=%s:sample_rates=%d",
                   av_get_sample_fmt_name(sys->format), ch_layout, infmt->i_rate);
    assert(ret < (int)sizeof(text_filter_str));

    AVFilterContext* aformat_ctx = avfilter_graph_alloc_filter(sys->filter_graph, aformat, "aformat");
    if ((ret = avfilter_init_str(aformat_ctx, text_filter_str)) < 0)
        goto end;

    if ((ret = avfilter_link(ctx_in, 0, aformat_ctx, 0)) < 0)
        goto end;
    ctx_in = aformat_ctx;

    if ((ret = avfilter_link(ctx_in, 0, sys->buffersink_ctx, 0)) < 0)
        goto end;

    if ((ret = avfilter_graph_config(sys->filter_graph, NULL)) < 0)
        goto end;


    static const struct vlc_filter_operations filter_ops =
    {
        .filter_audio = DoWork,
        .close = Close,
    };
    p_filter->ops = &filter_ops;
    p_filter->p_sys = sys;
    return VLC_SUCCESS;

end:
    return VLC_EGENERIC;
}
