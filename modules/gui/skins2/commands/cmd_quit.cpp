// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_quit.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include <vlc_vout.h>
#include <vlc_vout_osd.h>

#include "cmd_quit.hpp"
#include "../src/os_factory.hpp"
#include "../src/os_loop.hpp"


void CmdQuit::execute()
{
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_osd_Message( player,
        VOUT_SPU_CHANNEL_OSD, "%s", _( "Quit" ) );

    // Kill libvlc
    libvlc_Quit( vlc_object_instance(getIntf()) );
}


void CmdExitLoop::execute()
{
    // Get the instance of OSFactory
    OSFactory *pOsFactory = OSFactory::instance( getIntf() );

    // Exit the main OS loop
    pOsFactory->getOSLoop()->exit();
}
