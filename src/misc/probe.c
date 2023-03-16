// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * probe.c : run-time service listing
 *****************************************************************************
 * Copyright © 2009 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_probe.h>
#include <vlc_modules.h>
#include "libvlc.h"

#undef vlc_probe
void *vlc_probe (vlc_object_t *obj,
                 const char *capability, size_t *restrict pcount)
{
    vlc_probe_t *probe = vlc_custom_create (obj, sizeof(*probe), "probe");
    if (unlikely(probe == NULL))
    {
        *pcount = 0;
        return NULL;
    }
    probe->list = NULL;
    probe->count = 0;

    module_t *mod = module_need (probe, capability, NULL, false);
    if (mod != NULL)
    {
        msg_Warn (probe, "probing halted");
        module_unneed (probe, mod);
    }

    void *ret = probe->list;
    *pcount = probe->count;
    vlc_object_delete(probe);
    return ret;
}
