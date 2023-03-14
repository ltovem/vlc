/*****************************************************************************
 * edummy.c: dummy encoder plugin for vlc.
 *****************************************************************************
 * Copyright (C) 2002 VLC authors and VideoLAN
 *
 * Authors: Gildas Bazin <gbazin@netcourrier.com>
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
#include <vlc_plugin.h>
#include <vlc_codec.h>

static int OpenVideoEncoder( vlc_object_t * );
static int OpenAudioEncoder( vlc_object_t * );

vlc_module_begin ()
    set_shortname( N_("Dummy") )
    set_description( N_("Dummy encoder") )
    set_capability( "video encoder", 0 )
    set_callback( OpenVideoEncoder )
    add_shortcut( "dummy" )

    add_submodule()
    set_capability( "audio encoder", 0 )
    set_callback( OpenAudioEncoder )
    add_shortcut( "dummy" )
vlc_module_end ()


/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static block_t *EncodeVideo( encoder_t *p_enc, picture_t *p_pict );
static block_t *EncodeAudio( encoder_t *p_enc, block_t *p_buf );

/*****************************************************************************
 * OpenVideoDecoder: open the dummy encoder.
 *****************************************************************************/
static int OpenVideoEncoder( vlc_object_t *p_this )
{
    encoder_t *p_enc = (encoder_t *)p_this;

    static const struct vlc_encoder_operations video_ops =
        { .encode_video = EncodeVideo };

    p_enc->ops = &video_ops;

    return VLC_SUCCESS;
}

/*****************************************************************************
 * OpenAudioDecoder: open the dummy encoder.
 *****************************************************************************/
static int OpenAudioEncoder( vlc_object_t *p_this )
{
    encoder_t *p_enc = (encoder_t *)p_this;

    static const struct vlc_encoder_operations audio_ops =
        { .encode_audio = EncodeAudio };

    p_enc->ops = &audio_ops;
    return VLC_SUCCESS;
}

/****************************************************************************
 * EncodeVideo: the whole thing
 ****************************************************************************/
static block_t *EncodeVideo( encoder_t *p_enc, picture_t *p_pict )
{
    VLC_UNUSED(p_enc); VLC_UNUSED(p_pict);
    return NULL;
}

/****************************************************************************
 * EncodeAudio: the whole thing
 ****************************************************************************/
static block_t *EncodeAudio( encoder_t *p_enc, block_t *p_buf )
{
    VLC_UNUSED(p_enc); VLC_UNUSED(p_buf);
    return NULL;
}
