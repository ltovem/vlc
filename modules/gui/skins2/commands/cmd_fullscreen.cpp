// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_fullscreen.cpp
 *****************************************************************************
 * Copyright (C) 2003-2009 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "cmd_fullscreen.hpp"

void CmdFullscreen::execute()
{
    vlc_player_t *player = vlc_playlist_GetPlayer( getPL() );
    vlc_player_vout_SetFullscreen( player, true );
}
