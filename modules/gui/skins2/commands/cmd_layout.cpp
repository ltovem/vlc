/*****************************************************************************
 * cmd_layout.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "cmd_layout.hpp"
#include "../src/top_window.hpp"
#include "../src/generic_layout.hpp"
#include "../src/theme.hpp"


CmdLayout::CmdLayout( intf_thread_t *pIntf, TopWindow &rWindow,
                      GenericLayout &rLayout )
    : CmdGeneric( pIntf ), m_rWindow( rWindow ), m_rLayout( rLayout ) { }


void CmdLayout::execute()
{
    auto &p_theme = getIntf()->p_sys->p_theme;
    if( p_theme )
        p_theme->getWindowManager().setActiveLayout( m_rWindow, m_rLayout );
}
