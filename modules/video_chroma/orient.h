/*****************************************************************************
 * orient.h: image reorientation video conversion
 *****************************************************************************
 * Copyright (C) 2010 Laurent Aimar
 * Copyright (C) 2012 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#include <stdint.h>

typedef void (*plane_transform_cb)(void *, ptrdiff_t, const void *, ptrdiff_t,
                                   int, int);

#define PLANE_TRANSFORM_MAX_ORDER 3

struct plane_transforms {
    plane_transform_cb hflip[PLANE_TRANSFORM_MAX_ORDER + 1];
    plane_transform_cb transpose[PLANE_TRANSFORM_MAX_ORDER + 1];
};

