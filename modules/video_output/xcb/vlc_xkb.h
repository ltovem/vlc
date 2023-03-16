// SPDX-License-Identifier: LGPL-2.1-or-later
/**
 * @file vlc_xkb.h
 * @brief XKeyboard symbol mappings for VLC
 */
/*****************************************************************************
 * Copyright © 2009 Rémi Denis-Courmont
 *****************************************************************************/

#include <stdint.h>

struct xkb_state;

uint_fast32_t vlc_xkb_get_one(struct xkb_state *state, uint_fast32_t keycode);

