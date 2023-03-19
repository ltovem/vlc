// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * deinterlace.c: ARMv6 SIMD deinterlacing functions
 *****************************************************************************
 * Copyright (C) 2022 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_cpu.h>
#include <vlc_plugin.h>
#include "../../../video_filter/deinterlace/merge.h"

void merge8_armv6(void *, const void *, const void *, size_t);
void merge16_armv6(void *, const void *, const void *, size_t);

static void Probe(void *data)
{
    if (vlc_CPU_ARMv6()) {
        struct deinterlace_functions *const f = data;

        f->merges[0] = merge8_armv6;
        f->merges[1] = merge16_armv6;
    }
}

vlc_module_begin()
    set_description("ARM SIMD optimisation for deinterlacing")
    set_cpu_funcs("deinterlace functions", Probe, 10)
vlc_module_end()
