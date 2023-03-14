/*****************************************************************************
 * spu.c: transcoding spu encoder
 *****************************************************************************
 * Copyright (C) 2003-2009 VLC authors and VideoLAN
 *               2018 VideoLabs, VideoLAN and VLC authors
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Gildas Bazin <gbazin@videolan.org>
 *          Jean-Paul Saman <jpsaman #_at_# m2x dot nl>
 *          Antoine Cellerier <dionoea at videolan dot org>
 *          Ilkka Ollakka <ileoo at videolan dot org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_modules.h>
#include <vlc_codec.h>
#include <vlc_sout.h>

#include "encoder.h"
#include "encoder_priv.h"

int transcode_encoder_spu_open( transcode_encoder_t *p_enc,
                                const transcode_encoder_config_t *p_cfg )
{
    p_enc->p_encoder->p_cfg = p_cfg->p_config_chain;
    p_enc->p_encoder->ops = NULL;
    p_enc->p_encoder->fmt_out.i_codec = p_cfg->i_codec;

    p_enc->p_encoder->p_module = module_need( p_enc->p_encoder, "spu encoder",
                                              p_cfg->psz_name, true );

    assert( p_enc->p_encoder->p_module == NULL ||
            p_enc->p_encoder->ops != NULL);

    return ( p_enc->p_encoder->p_module ) ? VLC_SUCCESS: VLC_EGENERIC;
}

block_t * transcode_encoder_spu_encode( transcode_encoder_t *p_enc, subpicture_t *p_spu )
{
    return vlc_encoder_EncodeSub( p_enc->p_encoder, p_spu );
}
