/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef WINDOWSTHEMEPROVIDER_HPP
#define WINDOWSTHEMEPROVIDER_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <vlc_common.h>

int WindowsThemeProviderOpen(vlc_object_t* p_this);
int SystemPaletteThemeProviderOpen(vlc_object_t* p_this);

#endif // WINDOWSTHEMEPROVIDER_HPP
