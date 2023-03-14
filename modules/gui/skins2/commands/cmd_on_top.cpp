/*****************************************************************************
 * cmd_on_top.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "cmd_on_top.hpp"
#include "../src/theme.hpp"
#include "../src/window_manager.hpp"


void CmdOnTop::execute()
{
    getIntf()->p_sys->p_theme->getWindowManager().toggleOnTop();
}

void CmdSetOnTop::execute()
{
    getIntf()->p_sys->p_theme->getWindowManager().setOnTop( m_ontop );
}
