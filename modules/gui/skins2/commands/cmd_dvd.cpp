// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_dvd.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "cmd_dvd.hpp"
#include <vlc_player.h>
#include <vlc_playlist.h>

void CmdDvdNextTitle::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_SelectNextTitle( player );
    vlc_playlist_Unlock( getPL() );
}


void CmdDvdPreviousTitle::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_SelectPrevTitle( player );
    vlc_playlist_Unlock( getPL() );
}


void CmdDvdNextChapter::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_SelectNextChapter( player );
    vlc_playlist_Unlock( getPL() );
}


void CmdDvdPreviousChapter::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_SelectPrevChapter( player );
    vlc_playlist_Unlock( getPL() );
}


void CmdDvdRootMenu::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_Navigate( player, VLC_PLAYER_NAV_MENU );
    vlc_playlist_Unlock( getPL() );
}

