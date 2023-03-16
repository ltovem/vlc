/*****************************************************************************
 * cmd_muxer.cpp
 *****************************************************************************
 * Copyright (C) 2005 the VideoLAN team
 *
 * Authors: Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "cmd_muxer.hpp"


void CmdMuxer::execute()
{
    cmdList_t::const_iterator it;
    for( it = m_list.begin(); it != m_list.end(); ++it )
        (*it)->execute();
}
