/*****************************************************************************
 * cmd_snapshot.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef CMD_SNAPSHOT_HPP
#define CMD_SNAPSHOT_HPP

#include "cmd_generic.hpp"


/// Command to snapshot VLC
DEFINE_COMMAND(Snapshot, "snapshot" )
DEFINE_COMMAND(ToggleRecord, "togglerecord" )
DEFINE_COMMAND(NextFrame, "nextframe" )

#endif
