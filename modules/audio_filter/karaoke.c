/*****************************************************************************
 * karaoke.c : karaoke mode
 *****************************************************************************
 * Copyright © 2011 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_aout.h>
#include <vlc_filter.h>
#include <vlc_plugin.h>

static int Open (vlc_object_t *);

vlc_module_begin ()
    set_shortname (N_("Karaoke"))
    set_description (N_("Simple Karaoke filter"))
    set_subcategory (SUBCAT_AUDIO_AFILTER)

    set_capability ("audio filter", 0)
    set_callback(Open)
vlc_module_end ()

static block_t *Process (filter_t *, block_t *);

static int Open (vlc_object_t *obj)
{
    filter_t *filter = (filter_t *)obj;

    if (filter->fmt_in.audio.i_channels != 2)
    {
        msg_Err (filter, "voice removal requires stereo");
        return VLC_EGENERIC;
    }

    filter->fmt_in.audio.i_format = VLC_CODEC_FL32;
    aout_FormatPrepare(&filter->fmt_in.audio);
    filter->fmt_out.audio = filter->fmt_in.audio;

    static const struct vlc_filter_operations filter_ops =
    {
        .filter_audio = Process,
    };
    filter->ops = &filter_ops;
    return VLC_SUCCESS;
}

static block_t *Process (filter_t *filter, block_t *block)
{
    const float factor = .70710678 /* 1. / sqrtf (2) */;
    float *spl = (float *)block->p_buffer;

    for (unsigned i = block->i_nb_samples; i > 0; i--)
    {
        float s = (spl[0] - spl[1]) * factor;

        *(spl++) = s;
        *(spl++) = s;
        /* TODO: set output format to mono */
    }
    (void) filter;
    return block;
}
