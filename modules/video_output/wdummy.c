/**
 * @file wdummy.c
 * @brief Dummy video window provider for legacy video plugins
 */
/*****************************************************************************
 * Copyright © 2009, 2018 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdarg.h>

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_window.h>

static int Enable(vlc_window_t *wnd, const vlc_window_cfg_t *cfg)
{
    vlc_window_ReportSize(wnd, cfg->width, cfg->height);
    return VLC_SUCCESS;
}

static const struct vlc_window_operations ops = {
    .enable = Enable,
    .resize = vlc_window_ReportSize,
};

static int Open(vlc_window_t *wnd)
{
    wnd->type = VLC_WINDOW_TYPE_DUMMY;
    wnd->ops = &ops;
    return VLC_SUCCESS;
}

vlc_module_begin()
    set_shortname(N_("Dummy window"))
    set_description(N_("Dummy window"))
    set_subcategory(SUBCAT_VIDEO_VOUT)
    set_capability("vout window", 1)
    set_callback(Open)
    add_shortcut("dummy")
vlc_module_end()
