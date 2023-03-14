/*****************************************************************************
 * volume.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *          Erwan Tulou      <erwan10 aT videolan Dot org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_player.h>
#include <vlc_playlist.h>
#include <vlc_aout.h>
#include "volume.hpp"
#include <math.h>

Volume::Volume( intf_thread_t *pIntf ): VarPercent( pIntf )
{
    // compute preferred step in [0.,1.] range
    m_step = config_GetFloat( "volume-step" ) / (float)AOUT_VOLUME_MAX;

    // set current volume from the playlist
    setVolume( 0.0f, false );
}


void Volume::set( float percentage, bool updateVLC )
{
    VarPercent::set( percentage );
    if( updateVLC )
    {
        vlc_player_t *player = vlc_playlist_GetPlayer( getPL());
        vlc_player_aout_SetVolume( player, getVolume() );
    }
}


void Volume::setVolume( float volume, bool updateVLC )
{
    // translate from [0.,AOUT_VOLUME_MAX/AOUT_VOLUME_DEFAULT] into [0.,1.]
    float percentage = (volume > 0.f ) ?
                       volume * AOUT_VOLUME_DEFAULT / AOUT_VOLUME_MAX :
                       0.f;
    set( percentage, updateVLC );
}


float Volume::getVolume() const
{
    // translate from [0.,1.] into [0.,AOUT_VOLUME_MAX/AOUT_VOLUME_DEFAULT]
    return get() * AOUT_VOLUME_MAX / AOUT_VOLUME_DEFAULT;
}


std::string Volume::getAsStringPercent() const
{
    int value = lround( getVolume() * 100. );
    // 0 <= value <= 200, so we need 4 chars
    char str[4];
    snprintf( str, 4, "%i", value );
    return std::string(str);
}

