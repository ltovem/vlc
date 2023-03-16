// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * renderer_discoverer_internal.h:  libvlc external API
 *****************************************************************************
 * Copyright © 2016 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef _LIBVLC_RENDERER_DISCOVERER_INTERNAL_H
#define _LIBVLC_RENDERER_DISCOVERER_INTERNAL_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_renderer_discovery.h>

vlc_renderer_item_t *
libvlc_renderer_item_to_vlc( libvlc_renderer_item_t *p_item );

#endif
