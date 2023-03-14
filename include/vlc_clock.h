/*****************************************************************************
 * vlc_clock.h: clock API
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_CLOCK_H
#define VLC_CLOCK_H 1

/**
 * The VLC clock API.
 *
 * The publicly exposed clock API is for now very restraint. For now, only a
 * subset of the clock is exposed and simplified for stream output modules.
 *
 * The actual clock implementation is mostly private for now as no other use
 * case is found.
 */

/**
 * Opaques VLC Clock types.
 */
typedef struct vlc_clock_main_t vlc_clock_main_t;
typedef struct vlc_clock_t vlc_clock_t;

#endif
