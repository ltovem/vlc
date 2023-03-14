/*****************************************************************************
 * vlc_rand.h: RNG
 *****************************************************************************
 * Copyright © 2007 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_RAND_H
# define VLC_RAND_H

/**
 * \file
 * This file defined random number generator function in vlc
 */

VLC_API void vlc_rand_bytes(void *buf, size_t len);

/* Interlocked (but not reproducible) functions for the POSIX PRNG */
VLC_API double vlc_drand48(void) VLC_USED;
VLC_API long vlc_lrand48(void) VLC_USED;
VLC_API long vlc_mrand48(void) VLC_USED;

#endif
