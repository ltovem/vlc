/*****************************************************************************
 * sincos.c: GNU sincos() & sincosf() replacements
 *****************************************************************************
 * Copyright © 2016 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <math.h>

void sincos(double r, double *restrict sr, double *restrict cr)
{
    *sr = sin(r);
    *cr = cos(r);
}

void sincosf(float r, float *restrict sr, float *restrict cr)
{
    *sr = sinf(r);
    *cr = cosf(r);
}
