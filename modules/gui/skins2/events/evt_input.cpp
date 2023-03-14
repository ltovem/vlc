/*****************************************************************************
 * evt_input.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "evt_input.hpp"
#include "vlc_actions.h"

const int
    EvtInput::kModNone=0,
    EvtInput::kModAlt=KEY_MODIFIER_ALT,
    EvtInput::kModShift=KEY_MODIFIER_SHIFT,
    EvtInput::kModCtrl=KEY_MODIFIER_CTRL,
    EvtInput::kModMeta=KEY_MODIFIER_META,
    EvtInput::kModCmd=KEY_MODIFIER_COMMAND;

EvtInput::EvtInput( intf_thread_t *pIntf, int mod )
    : EvtGeneric( pIntf), m_mod( mod ) { }


void EvtInput::addModifier( std::string &rEvtString ) const
{
    if( m_mod == kModNone )
    {
        rEvtString += ":none";
    }
    else
    {
        std::string m = ":";
        if( m_mod & kModAlt )
            m += "alt,";
        if( m_mod & kModCtrl )
            m += "ctrl,";
        if( m_mod & kModShift )
            m += "shift,";
        if( m_mod & kModMeta )
            m += "meta,";
        if( m_mod & kModCmd )
            m += "cmd,";
        // Append the result except the last ','
        rEvtString.insert( rEvtString.end(), m.begin(), m.end()-1 );
    }
}
