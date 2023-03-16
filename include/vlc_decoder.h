// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * vlc_decoder.h: decoder API
 *****************************************************************************
 * Copyright (C) 1999-2015 VLC authors and VideoLAN
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *          Laurent Aimar <fenrir@via.ecp.fr>
 *****************************************************************************/

#ifndef VLC_DECODER_H
#define VLC_DECODER_H 1

/**
 * \ingroup decoder
 * @{
 */

#include <vlc_subpicture.h>

typedef struct vlc_input_decoder_t vlc_input_decoder_t;

/**
 * This defines an opaque input resource handler.
 */
typedef struct input_resource_t input_resource_t;

/* */
struct vlc_clock_t;
VLC_API vlc_input_decoder_t *
vlc_input_decoder_Create( vlc_object_t *, const es_format_t *,
                          struct vlc_clock_t *, input_resource_t * ) VLC_USED;
VLC_API void vlc_input_decoder_Delete( vlc_input_decoder_t * );
VLC_API void vlc_input_decoder_Decode( vlc_input_decoder_t *, block_t *, bool b_do_pace );
VLC_API void vlc_input_decoder_Drain( vlc_input_decoder_t * );
VLC_API void vlc_input_decoder_Flush( vlc_input_decoder_t * );
VLC_API int  vlc_input_decoder_SetSpuHighlight( vlc_input_decoder_t *, const vlc_spu_highlight_t * );
VLC_API void vlc_input_decoder_ChangeDelay( vlc_input_decoder_t *, vlc_tick_t i_delay );

/**
 * It creates an empty input resource handler.
 *
 * The given object MUST stay alive as long as the input_resource_t is
 * not deleted.
 */
VLC_API input_resource_t * input_resource_New( vlc_object_t * ) VLC_USED;

/**
 * It releases an input resource.
 */
VLC_API void input_resource_Release( input_resource_t * );

/** @} */
#endif
