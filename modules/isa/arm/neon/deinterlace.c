// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * deinterlace.c: ARM NEON deinterlacing functions
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

void merge8_arm_neon(void *, const void *, const void *, size_t);
void merge16_arm_neon(void *, const void *, const void *, size_t);

static void Probe(void *data)
{
    if (vlc_CPU_ARM_NEON()) {
        struct deinterlace_functions *const f = data;

        f->merges[0] = merge8_arm_neon;
        f->merges[1] = merge16_arm_neon;
    }
}

vlc_module_begin()
    set_description("ARM NEON optimisation for deinterlacing")
    set_cpu_funcs("deinterlace functions", Probe, 20)
vlc_module_end()
