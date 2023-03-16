// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * evt_mouse.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "evt_mouse.hpp"


const std::string EvtMouse::getAsString() const
{
    std::string event = "mouse";

    // Add the button
    if( m_button == kLeft )
        event += ":left";
    else if( m_button == kMiddle )
        event += ":middle";
    else if( m_button == kRight )
        event += ":right";
    else
        msg_Warn( getIntf(), "unknown button type" );

    // Add the action
    if( m_action == kDown )
        event += ":down";
    else if( m_action == kUp )
        event += ":up";
    else if( m_action == kDblClick )
        event += ":dblclick";
    else
        msg_Warn( getIntf(), "unknown action type" );

    // Add the modifier
    addModifier( event );

    return event;
}

