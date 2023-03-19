// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_change_skin.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "cmd_change_skin.hpp"
#include "cmd_quit.hpp"
#include "../src/os_factory.hpp"
#include "../src/os_loop.hpp"
#include "../src/theme.hpp"
#include "../src/theme_loader.hpp"
#include "../src/theme_repository.hpp"
#include "../src/window_manager.hpp"
#include "../src/vout_manager.hpp"
#include "../src/vlcproc.hpp"


void CmdChangeSkin::execute()
{
    // Save the old theme to restore it in case of problem
    auto pOldTheme = std::move(getIntf()->p_sys->p_theme);

    if( pOldTheme )
    {
        pOldTheme->getWindowManager().saveVisibility();
        pOldTheme->getWindowManager().hideAll();
    }

    VoutManager::instance( getIntf() )->saveVoutConfig();

    ThemeLoader loader( getIntf() );
    if( loader.load( m_file ) )
    {
        // Everything went well
        msg_Info( getIntf(), "new theme successfully loaded (%s)",
                 m_file.c_str() );
        pOldTheme.reset();

        // restore vout config
        VoutManager::instance( getIntf() )->restoreVoutConfig( true );
    }
    else if( pOldTheme )
    {
        msg_Warn( getIntf(), "a problem occurred when loading the new theme,"
                  " restoring the previous one" );
        getIntf()->p_sys->p_theme = std::move(pOldTheme);
        VoutManager::instance( getIntf() )->restoreVoutConfig( false );
        getIntf()->p_sys->p_theme->getWindowManager().restoreVisibility();
    }
    else
    {
        msg_Err( getIntf(), "cannot load the theme, aborting" );
        // Quit
        CmdQuit cmd( getIntf() );
        cmd.execute();
    }

   // update the repository
   ThemeRepository::instance( getIntf() )->updateRepository();
}

