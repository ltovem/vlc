// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * evt_scroll.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "evt_scroll.hpp"


const std::string EvtScroll::getAsString() const
{
    std::string event = "scroll";

    // Add the direction
    if( m_direction == kUp )
        event += ":up";
    else if( m_direction == kDown )
        event += ":down";
    else
        msg_Warn( getIntf(), "unknown scrolling direction" );

    // Add the modifier
    addModifier( event );

    return event;
}

