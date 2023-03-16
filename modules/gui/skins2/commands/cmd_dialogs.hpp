// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_dialogs.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef CMD_DIALOGS_HPP
#define CMD_DIALOGS_HPP

#include "cmd_generic.hpp"
#include "../src/dialogs.hpp"
#include "cmd_change_skin.hpp"

#include <vlc_interface.h>


#define DEFC( a, c ) \
class CmdDlg##a: public CmdGeneric                              \
{   public:                                                     \
    CmdDlg##a( intf_thread_t *pIntf ): CmdGeneric( pIntf ) { }  \
    virtual ~CmdDlg##a() { }                                    \
    virtual void execute()                                      \
    {                                                           \
        Dialogs *dlg = Dialogs::instance( getIntf() );          \
        if( dlg ) dlg->c;                                       \
    }                                                           \
    virtual std::string getType() const { return #a" dialog"; }      \
};

DEFC( ChangeSkin,         showChangeSkin() )
DEFC( FileSimple,         showFileSimple( true ) )
DEFC( File,               showFile( true ) )
DEFC( Disc,               showDisc( true ) )
DEFC( Net,                showNet( true ) )
DEFC( Messages,           showMessages() )
DEFC( Prefs,              showPrefs() )
DEFC( FileInfo,           showFileInfo() )

DEFC( Add,                showFile( false ) )
DEFC( PlaylistLoad,       showPlaylistLoad() )
DEFC( PlaylistSave,       showPlaylistSave() )
DEFC( Directory,          showDirectory( true ) )
DEFC( StreamingWizard,    showStreamingWizard() )
DEFC( Playlist,           showPlaylist() )

DEFC( ShowPopupMenu,      showPopupMenu(true,INTF_DIALOG_POPUPMENU) )
DEFC( HidePopupMenu,      showPopupMenu(false,INTF_DIALOG_POPUPMENU) )
DEFC( ShowAudioPopupMenu, showPopupMenu(true,INTF_DIALOG_AUDIOPOPUPMENU) )
DEFC( HideAudioPopupMenu, showPopupMenu(false,INTF_DIALOG_AUDIOPOPUPMENU) )
DEFC( ShowVideoPopupMenu, showPopupMenu(true,INTF_DIALOG_VIDEOPOPUPMENU) )
DEFC( HideVideoPopupMenu, showPopupMenu(false,INTF_DIALOG_VIDEOPOPUPMENU) )
DEFC( ShowMiscPopupMenu,  showPopupMenu(true,INTF_DIALOG_MISCPOPUPMENU) )
DEFC( HideMiscPopupMenu,  showPopupMenu(false,INTF_DIALOG_MISCPOPUPMENU) )

#undef DEFC

#endif
