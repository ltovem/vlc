// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_input.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "cmd_input.hpp"
#include "cmd_dialogs.hpp"
#include <vlc_player.h>
#include <vlc_playlist.h>

void CmdPlay::execute()
{
    vlc_playlist_Lock( getPL() );
    bool b_empty = (vlc_playlist_Count( getPL() ) == 0 );
    if( !b_empty )
    {
        vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
        if( vlc_player_IsStarted( player ) )
            vlc_playlist_Resume( getPL() );
        else
            vlc_playlist_PlayAt( getPL(), 0 );
        vlc_playlist_Unlock( getPL() );
    }
    else
    {
        vlc_playlist_Unlock( getPL() );
        CmdDlgFile( getIntf() ).execute();
    }
}


void CmdPause::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_playlist_Pause( getPL() );
    vlc_playlist_Unlock( getPL() );
}


void CmdStop::execute()
{
    vlc_playlist_Lock( getPL() );
#if 0
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    if( vlc_player_IsStarted( player ) )
        vlc_playlist_Stop( getPL() );
    else
        vlc_playlist_GoTo( getPL(), 0 );
#endif
    vlc_playlist_Stop( getPL() );
    vlc_playlist_GoTo( getPL(), -1 );

    vlc_playlist_Unlock( getPL() );
}


void CmdSlower::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_DecrementRate( player );
    vlc_playlist_Unlock( getPL() );
}


void CmdFaster::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_IncrementRate( player );
    vlc_playlist_Unlock( getPL() );
}


void CmdMute::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_aout_ToggleMute( player );
    vlc_playlist_Unlock( getPL() );
}


void CmdVolumeUp::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_aout_IncrementVolume( player, 1, NULL );
    vlc_playlist_Unlock( getPL() );
}


void CmdVolumeDown::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_aout_DecrementVolume( player, 1, NULL );
    vlc_playlist_Unlock( getPL() );
}

