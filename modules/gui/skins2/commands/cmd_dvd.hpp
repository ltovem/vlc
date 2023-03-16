// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * cmd_dvd.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef CMD_DVD_HPP
#define CMD_DVD_HPP

#include "cmd_generic.hpp"

/// Commands to control the input
DEFINE_COMMAND( DvdNextTitle, "next title" )
DEFINE_COMMAND( DvdPreviousTitle, "previous title" )
DEFINE_COMMAND( DvdNextChapter, "next chapter" )
DEFINE_COMMAND( DvdPreviousChapter, "previous chapter" )
DEFINE_COMMAND( DvdRootMenu, "root menu" )


#endif
