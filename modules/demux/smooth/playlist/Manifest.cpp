// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Manifest.cpp
 *****************************************************************************
 Copyright © 2015 - VideoLAN and VLC Authors
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "Manifest.hpp"

#include <vlc_common.h>

using namespace adaptive;
using namespace smooth::playlist;

Manifest::Manifest (vlc_object_t *p_object) :
    BasePlaylist(p_object)
{
    minUpdatePeriod.Set( VLC_TICK_FROM_SEC(5) );
    addAttribute(new TimescaleAttr(Timescale(10000000))); // 100ns
    b_live = false;
}

Manifest::~Manifest()
{

}

bool Manifest::isLive() const
{
    return b_live;
}
