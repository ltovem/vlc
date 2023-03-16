/*****************************************************************************
 * cmd_resize.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "cmd_resize.hpp"
#include "../src/generic_layout.hpp"
#include "../src/vlcproc.hpp"
#include "../src/window_manager.hpp"
#include "../src/vout_manager.hpp"
#include "../controls/ctrl_video.hpp"


CmdResize::CmdResize( intf_thread_t *pIntf, const WindowManager &rWindowManager,
                      GenericLayout &rLayout, int width, int height )
    : CmdGeneric( pIntf ), m_rWindowManager( rWindowManager ),
      m_rLayout( rLayout ), m_width( width ), m_height( height ) { }


void CmdResize::execute()
{
    // Resize the layout
    m_rWindowManager.resize( m_rLayout, m_width, m_height );
}



CmdResizeVout::CmdResizeVout( intf_thread_t *pIntf, vlc_window_t* pWnd,
                              int width, int height )
    : CmdGeneric( pIntf ), m_pWnd( pWnd ), m_width( width ),
      m_height( height ) { }


void CmdResizeVout::execute()
{
    getIntf()->p_sys->p_voutManager->setSizeWnd( m_pWnd, m_width, m_height );
}

CmdSetFullscreen::CmdSetFullscreen( intf_thread_t *pIntf,
                                    vlc_window_t * pWnd, bool fullscreen )
    : CmdGeneric( pIntf ), m_pWnd( pWnd ), m_bFullscreen( fullscreen ) { }


void CmdSetFullscreen::execute()
{
    getIntf()->p_sys->p_voutManager->setFullscreenWnd( m_pWnd, m_bFullscreen );
}
