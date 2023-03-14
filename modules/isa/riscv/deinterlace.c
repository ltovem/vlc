/*****************************************************************************
 * deinterlace.c: RISC-V V deinterlacing functions
 *****************************************************************************
 * Copyright (C) 2022 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_cpu.h>
#include <vlc_plugin.h>
#include "../../video_filter/deinterlace/merge.h"

void merge8_rvv(void *, const void *, const void *, size_t);
void merge16_rvv(void *, const void *, const void *, size_t);

static void Probe(void *data)
{
    if (vlc_CPU_RV_V()) {
        struct deinterlace_functions *const f = data;

        f->merges[0] = merge8_rvv;
        f->merges[1] = merge16_rvv;
    }
}

vlc_module_begin()
    set_description("RISC-V V optimisation for deinterlacing")
    set_cpu_funcs("deinterlace functions", Probe, 10)
vlc_module_end()
