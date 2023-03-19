// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_snapshot.cpp
 *****************************************************************************
 * Copyright (C) 2006-2009 the VideoLAN team
 *
 * Authors: Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "cmd_snapshot.hpp"
#include <vlc_player.h>

void CmdSnapshot::execute()
{
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_vout_Snapshot( player );
}


void CmdToggleRecord::execute()
{
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_ToggleRecording( player );
}


void CmdNextFrame::execute()
{
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_NextVideoFrame( player );
}


