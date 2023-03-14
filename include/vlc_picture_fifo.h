/*****************************************************************************
 * vlc_picture_fifo.h: picture fifo definitions
 *****************************************************************************
 * Copyright (C) 2009 VLC authors and VideoLAN
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_PICTURE_FIFO_H
#define VLC_PICTURE_FIFO_H 1

/**
 * \file
 * This file defines picture fifo structures and functions in vlc
 */

#include <vlc_picture.h>

/**
 * Picture fifo handle
 *
 * It is thread safe (push/pop).
 */
typedef struct picture_fifo_t picture_fifo_t;

/**
 * It creates an empty picture_fifo_t.
 */
VLC_API picture_fifo_t * picture_fifo_New( void ) VLC_USED;

/**
 * It destroys a fifo created by picture_fifo_New.
 *
 * All pictures inside the fifo will be released by picture_Release.
 */
VLC_API void picture_fifo_Delete( picture_fifo_t * );

/**
 * It retrieves a picture_t from the fifo.
 *
 * If the fifo is empty, it return NULL without waiting.
 */
VLC_API picture_t * picture_fifo_Pop( picture_fifo_t * ) VLC_USED;

/**
 * It returns whether the fifo is empty or not.
 */
VLC_API bool picture_fifo_IsEmpty( picture_fifo_t * );

/**
 * It saves a picture_t into the fifo.
 */
VLC_API void picture_fifo_Push( picture_fifo_t *, picture_t * );

/**
 * It release all picture inside the fifo that have a lower or equal date
 * if flush_before or higher or equal to if not flush_before than the given one.
 * Passing VLC_TICK_INVALID on the date releases all the pictures.
 *
 * All pictures inside the fifo will be released by picture_Release.
 */
VLC_API void picture_fifo_Flush( picture_fifo_t *, vlc_tick_t date, bool flush_before );

#endif /* VLC_PICTURE_FIFO_H */

