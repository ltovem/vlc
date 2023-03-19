// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * evt_special.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "evt_special.hpp"


const std::string EvtSpecial::getAsString() const
{
    std::string event = "special";

    // Add the action
    if( m_action == kShow )
        event += ":show";
    else if( m_action == kHide )
        event += ":hide";
    else if( m_action == kEnable )
        event += ":enable";
    else if( m_action == kDisable )
        event += ":disable";
    else
        msg_Warn( getIntf(), "unknown action type" );

    return event;
}

