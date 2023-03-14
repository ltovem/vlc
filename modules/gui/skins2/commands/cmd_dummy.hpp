/*****************************************************************************
 * cmd_dummy.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef CMD_DUMMY_HPP
#define CMD_DUMMY_HPP

#include "cmd_generic.hpp"


/// Dummy command
DEFINE_COMMAND( Dummy, "dummy" )

void CmdDummy::execute() {}

#endif
