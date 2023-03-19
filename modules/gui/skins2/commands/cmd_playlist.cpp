// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_playlist.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "cmd_playlist.hpp"
#include <vlc_playlist.h>
#include <vlc_url.h>
#include "../src/vlcproc.hpp"
#include "../utils/var_bool.hpp"

void CmdPlaylistDel::execute()
{
    m_rList.delSelected();
}

void CmdPlaylistNext::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_playlist_Next( getPL() );
    vlc_playlist_Unlock( getPL() );
}


void CmdPlaylistPrevious::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_playlist_Prev( getPL() );
    vlc_playlist_Unlock( getPL() );
}


void CmdPlaylistRandom::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_playlist_SetPlaybackOrder( getPL(), m_value ?
                    VLC_PLAYLIST_PLAYBACK_ORDER_RANDOM :
                    VLC_PLAYLIST_PLAYBACK_ORDER_NORMAL );
    vlc_playlist_Unlock( getPL() );
}


void CmdPlaylistLoop::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_playlist_SetPlaybackRepeat( getPL(), m_value ?
                    VLC_PLAYLIST_PLAYBACK_REPEAT_ALL :
                    VLC_PLAYLIST_PLAYBACK_REPEAT_NONE );
    vlc_playlist_Unlock( getPL() );
}


void CmdPlaylistRepeat::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_playlist_SetPlaybackRepeat( getPL(), m_value ?
                    VLC_PLAYLIST_PLAYBACK_REPEAT_CURRENT :
                    VLC_PLAYLIST_PLAYBACK_REPEAT_NONE);
    vlc_playlist_Unlock( getPL() );
}


void CmdPlaylistLoad::execute()
{
    char* psz_path = vlc_uri2path( m_file.c_str() );
    if ( !psz_path )
    {
        msg_Err(getIntf(),"unable to load playlist %s", m_file.c_str() );
        return;
    }
    free( psz_path );

    input_item_t *media = input_item_New( m_file.c_str(), NULL );
    vlc_playlist_Lock( getPL() );
    vlc_playlist_AppendOne( getPL(), media );
    ssize_t index = vlc_playlist_IndexOfMedia( getPL(), media );
    if( index != -1 )
        vlc_playlist_PlayAt( getPL(), index) ;
    vlc_playlist_Unlock( getPL() );
}


void CmdPlaylistSave::execute()
{
    const char *psz_module;
    if( m_file.find( ".xsp", 0 ) != std::string::npos )
        psz_module = "export-xspf";
    else if( m_file.find( "m3u", 0 ) != std::string::npos )
        psz_module = "export-m3u";
    else if( m_file.find( "html", 0 ) != std::string::npos )
        psz_module = "export-html";
    else
    {
        msg_Err(getIntf(),"Did not recognise playlist export file type");
        return;
    }

    vlc_playlist_Lock( getPL() );
    vlc_playlist_Export( getPL(), m_file.c_str(), psz_module );
    vlc_playlist_Unlock( getPL() );
}

void CmdPlaylistFirst::execute()
{
    vlc_playlist_Lock( getPL() );
    vlc_playlist_PlayAt( getPL(), 0 );
    vlc_playlist_Unlock( getPL() );
}
