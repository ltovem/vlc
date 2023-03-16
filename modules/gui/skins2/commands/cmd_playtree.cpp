// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_playtree.cpp
 *****************************************************************************
 * Copyright (C) 2005 the VideoLAN team
 *
 * Authors: Antoine Cellerier <dionoea@videolan.org>
 *          Clément Stenac <zorglub@videolan.org>
 *****************************************************************************/

#include "cmd_playtree.hpp"
#include "../src/vlcproc.hpp"
#include "../utils/var_bool.hpp"

void CmdPlaytreeDel::execute()
{
    m_rTree.delSelected();
}

void CmdPlaytreeSort::execute()
{
    /// \todo Choose sort method/order - Need more commands
    /// \todo Choose the correct view
    const struct vlc_playlist_sort_criterion option = {
        .key = VLC_PLAYLIST_SORT_KEY_TITLE,
        .order = VLC_PLAYLIST_SORT_ORDER_ASCENDING };
    vlc_playlist_Lock( getPL() );
    vlc_playlist_Sort( getPL(), &option, 1 );
    vlc_playlist_Unlock( getPL() );
}

void CmdPlaytreeReset::execute()
{
    VlcProc::instance( getIntf() )->getPlaytreeVar().onChange();
}
