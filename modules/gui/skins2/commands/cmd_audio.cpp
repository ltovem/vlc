// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_audio.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *****************************************************************************/

#include "cmd_audio.hpp"
#include "../src/vlcproc.hpp"
#include <vlc_playlist.h>
#include <vlc_player.h>
#include <string>

void CmdSetEqualizer::execute()
{
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    (void)vlc_player_aout_EnableFilter( player, "equalizer", m_enable );
}


