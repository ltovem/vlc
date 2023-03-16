// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * tdummy.c : dummy text rendering functions
 *****************************************************************************
 * Copyright (C) 2000, 2001 VLC authors and VideoLAN
 *
 * Authors: Sigmund Augdal Helberg <dnumgis@videolan.org>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_filter.h>

static int OpenRenderer( filter_t * );

vlc_module_begin ()
    set_shortname( N_("Dummy") )
    set_description( N_("Dummy font renderer") )
    set_callback_text_renderer( OpenRenderer, 1 )
vlc_module_end ()


static int RenderText( filter_t *p_filter, subpicture_region_t *p_region_out,
                       subpicture_region_t *p_region_in,
                       const vlc_fourcc_t *p_chroma_list )
{
    VLC_UNUSED(p_filter); VLC_UNUSED(p_region_out); VLC_UNUSED(p_region_in);
    VLC_UNUSED(p_chroma_list);
    return VLC_EGENERIC;
}

static const struct vlc_filter_operations filter_ops = {
    .render = RenderText,
};

static int OpenRenderer( filter_t *p_filter )
{
    p_filter->ops = &filter_ops;
    return VLC_SUCCESS;
}
