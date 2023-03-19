// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_quit.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef CMD_QUIT_HPP
#define CMD_QUIT_HPP

#include "cmd_generic.hpp"


/// "Quit" command
DEFINE_COMMAND( Quit, "quit" )

/// "ExitLoop" command
DEFINE_COMMAND( ExitLoop, "exitloop" )

#endif
