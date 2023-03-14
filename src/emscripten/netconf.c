/*****************************************************************************
 * vlc_getProxyUrl for emscripten
 *****************************************************************************
 * Copyright (C) 2021 - VideoLabs, VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <vlc_common.h>
#include <vlc_network.h>

char *vlc_getProxyUrl(const char *url)
{
    VLC_UNUSED(url);
    return NULL;
}
