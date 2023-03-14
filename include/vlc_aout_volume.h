/*****************************************************************************
 * vlc_aout_volume.h: audio volume module
 *****************************************************************************
 * Copyright (C) 2002-2009 VLC authors and VideoLAN
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *          Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_AOUT_MIXER_H
#define VLC_AOUT_MIXER_H 1

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup audio_volume Audio output volume
 * \ingroup audio_output
 * @{
 * \file
 * This file defines functions, structures and macros for audio output mixer object
 */

typedef struct audio_volume audio_volume_t;

/**
 * Audio volume
 */
struct audio_volume
{
    struct vlc_object_t obj;

    vlc_fourcc_t format; /**< Audio samples format */
    void (*amplify)(audio_volume_t *, block_t *, float); /**< Amplifier */
};

/** @} */

#ifdef __cplusplus
}
#endif

#endif
