/*****************************************************************************
 * cmd_minimize.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Mohammed Adnène Trojette     <adn@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "cmd_minimize.hpp"
#include "../src/window_manager.hpp"
#include "../src/os_factory.hpp"


void CmdMinimize::execute()
{
    OSFactory::instance( getIntf() )->minimize();
}


void CmdRestore::execute()
{
    OSFactory::instance( getIntf() )->restore();
}


CmdMaximize::CmdMaximize( intf_thread_t *pIntf, WindowManager &rWindowManager,
                          TopWindow &rWindow )
    : CmdGeneric( pIntf ), m_rWindowManager( rWindowManager ),
      m_rWindow( rWindow ) { }


void CmdMaximize::execute()
{
    // Simply delegate the job to the WindowManager
    m_rWindowManager.maximize( m_rWindow );
}


CmdUnmaximize::CmdUnmaximize( intf_thread_t *pIntf,
                              WindowManager &rWindowManager,
                              TopWindow &rWindow )
    : CmdGeneric( pIntf ), m_rWindowManager( rWindowManager ),
      m_rWindow( rWindow ) { }


void CmdUnmaximize::execute()
{
    // Simply delegate the job to the WindowManager
    m_rWindowManager.unmaximize( m_rWindow );
}


void CmdAddInTray::execute()
{
    OSFactory::instance( getIntf() )->addInTray();
}


void CmdRemoveFromTray::execute()
{
    OSFactory::instance( getIntf() )->removeFromTray();
}


void CmdAddInTaskBar::execute()
{
    OSFactory::instance( getIntf() )->addInTaskBar();
}


void CmdRemoveFromTaskBar::execute()
{
    OSFactory::instance( getIntf() )->removeFromTaskBar();
}

