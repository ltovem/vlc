// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * evt_key.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "evt_key.hpp"
#include <vlc_actions.h>


const std::string EvtKey::getAsString() const
{
    std::string event = "key";

    // Add the action
    if( m_action == kDown )
        event += ":down";
    else if( m_action == kUp )
        event += ":up";
    else
        msg_Warn( getIntf(), "Unknown action type" );

    // Add the key
    char *keyName = vlc_keycode2str( m_key & ~KEY_MODIFIER, true );
    if( keyName )
    {
        event += std::string(":") + keyName;
        free( keyName );
    }
    else
        msg_Warn( getIntf(), "Unknown key: %d", m_key );

    // Add the modifier
    addModifier( event );

    return event;
}
