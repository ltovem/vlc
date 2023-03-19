// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * mixer.c: RISC-V V audio volume mixer module
 *****************************************************************************
 * Copyright (C) 2012, 2022 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>
#include <math.h>
#include <stdint.h>

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_cpu.h>
#include <vlc_aout.h>
#include <vlc_aout_volume.h>

void rvv_amplify_f32(void *, const void *, size_t, float);
void rvv_amplify_f64(void *, const void *, size_t, double);
void rvv_amplify_i16(void *, const void *, size_t, uint16_t);
void rvv_amplify_i32(void *, const void *, size_t, uint32_t);
void rvv_amplify_u8(void *, const void *, size_t, uint8_t);

static void AmplifyFloat(audio_volume_t *volume, block_t *block, float amp)
{
    void *buf = block->p_buffer;

    if (amp != 1.f)
        rvv_amplify_f32(buf, buf, block->i_buffer, amp);

    (void) volume;
}

static void AmplifyDouble(audio_volume_t *volume, block_t *block, float amp)
{
    void *buf = block->p_buffer;

    if (amp != 1.f)
        rvv_amplify_f64(buf, buf, block->i_buffer, amp);

    (void) volume;
}

static void AmplifyShort(audio_volume_t *volume, block_t *block, float amp)
{
    void *buf = block->p_buffer;
    uint_fast16_t fixed_amp = lroundf(ldexpf(amp, 16));

    if (amp != 1.f)
        rvv_amplify_i16(buf, buf, block->i_buffer, fixed_amp);

    (void) volume;
}

static void AmplifyInt(audio_volume_t *volume, block_t *block, float amp)
{
    void *buf = block->p_buffer;
    uint_fast32_t fixed_amp = lroundf(ldexpf(amp, 32));

    if (amp != 1.f)
        rvv_amplify_i32(buf, buf, block->i_buffer, fixed_amp);

    (void) volume;
}

static void AmplifyByte(audio_volume_t *volume, block_t *block, float amp)
{
    void *buf = block->p_buffer;
    uint_fast8_t fixed_amp = lroundf(ldexpf(amp, 8));

    if (amp != 1.f)
        rvv_amplify_u8(buf, buf, block->i_buffer, fixed_amp);

    (void) volume;
}

static int Probe(vlc_object_t *obj)
{
    audio_volume_t *volume = (audio_volume_t *)obj;

    if (!vlc_CPU_RV_V())
        return VLC_ENOTSUP;

    switch (volume->format) {
        case VLC_CODEC_FL32:
            volume->amplify = AmplifyFloat;
            break;

        case VLC_CODEC_FL64:
            volume->amplify = AmplifyDouble;
            break;

        case VLC_CODEC_S16N:
            volume->amplify = AmplifyShort;
            break;

        case VLC_CODEC_S32N:
            volume->amplify = AmplifyInt;
            break;

        case VLC_CODEC_U8:
            volume->amplify = AmplifyByte;
            break;

        default:
            return VLC_ENOTSUP;
    }

    return VLC_SUCCESS;
}

vlc_module_begin()
    set_subcategory(SUBCAT_AUDIO_AFILTER)
    set_description("RISC-V V optimisation for audio volume")
    set_capability("audio volume", 20)
    set_callback(Probe)
vlc_module_end()
