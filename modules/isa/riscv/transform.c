// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * transform.c: RISC-V V video transforms
 *****************************************************************************
 * Copyright (C) 2022 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_cpu.h>
#include <vlc_plugin.h>
#include "../../video_chroma/orient.h"

void rvv_hflip_8(void *, ptrdiff_t, const void *, ptrdiff_t, int, int);
void rvv_hflip_16(void *, ptrdiff_t, const void *, ptrdiff_t, int, int);
void rvv_hflip_32(void *, ptrdiff_t, const void *, ptrdiff_t, int, int);
void rvv_hflip_64(void *, ptrdiff_t, const void *, ptrdiff_t, int, int);
void rvv_transpose_8(void *, ptrdiff_t, const void *, ptrdiff_t, int, int);
void rvv_transpose_16(void *, ptrdiff_t, const void *, ptrdiff_t, int, int);
void rvv_transpose_32(void *, ptrdiff_t, const void *, ptrdiff_t, int, int);
void rvv_transpose_64(void *, ptrdiff_t, const void *, ptrdiff_t, int, int);

static void Probe(void *data)
{
    if (vlc_CPU_RV_V()) {
        struct plane_transforms *const transforms = data;

        transforms->hflip[0] = rvv_hflip_8;
        transforms->hflip[1] = rvv_hflip_16;
        transforms->hflip[2] = rvv_hflip_32;
        transforms->hflip[3] = rvv_hflip_64;
        transforms->transpose[0] = rvv_transpose_8;
        transforms->transpose[1] = rvv_transpose_16;
        transforms->transpose[2] = rvv_transpose_32;
        transforms->transpose[3] = rvv_transpose_64;
    }
}

vlc_module_begin()
    set_subcategory(SUBCAT_VIDEO_VFILTER)
    set_description("RISC-V V optimisation for video transform")
    set_cpu_funcs("video transform", Probe, 10)
vlc_module_end()
