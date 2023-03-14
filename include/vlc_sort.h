/******************************************************************************
 * vlc_sort.h
 ******************************************************************************
 * Copyright © 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_SORT_H
#define VLC_SORT_H

#include <stdlib.h>
#include <stddef.h>

/**
 * Sort an array with reentrancy, following the upcoming POSIX prototype
 *
 * cf. POSIX qsort_r
 */
VLC_API void vlc_qsort(void *base, size_t nmemb, size_t size,
                       int (*compar)(const void *, const void *, void *),
                       void *arg);

#endif
