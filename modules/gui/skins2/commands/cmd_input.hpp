// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_input.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef CMD_INPUT_HPP
#define CMD_INPUT_HPP

#include "cmd_generic.hpp"

/// Commands to control the input
DEFINE_COMMAND( Play, "play" )
DEFINE_COMMAND( Pause, "pause" )
DEFINE_COMMAND( Stop, "stop" )
DEFINE_COMMAND( Slower, "slower" )
DEFINE_COMMAND( Faster, "faster" )
DEFINE_COMMAND( Mute, "mute" )
DEFINE_COMMAND( VolumeUp, "volume up" )
DEFINE_COMMAND( VolumeDown, "volume down" )


#endif
