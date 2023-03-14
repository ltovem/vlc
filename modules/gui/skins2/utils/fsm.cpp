/*****************************************************************************
 * fsm.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "fsm.hpp"
#include "../commands/cmd_generic.hpp"


void FSM::addState( const std::string &state )
{
    m_states.insert( state );
}


void FSM::addTransition( const std::string &state1, const std::string &event,
                         const std::string &state2, CmdGeneric *pCmd )
{
    // Check that we already know the states
    if( m_states.find( state1 ) == m_states.end() ||
        m_states.find( state2 ) == m_states.end() )
    {
        msg_Warn( getIntf(),
                  "FSM: ignoring transition between invalid states" );
        return;
    }

    Key_t key( state1, event );
    Data_t data( state2, pCmd );

    // Check that the transition doesn't already exist
    if( m_transitions.find( key ) != m_transitions.end() )
    {
        msg_Warn( getIntf(), "FSM: transition already exists" );
        return;
    }

    m_transitions[key] = data;
}


void FSM::setState( const std::string &state )
{
    if( m_states.find( state ) == m_states.end() )
    {
        msg_Warn( getIntf(), "FSM: trying to set an invalid state" );
        return;
    }
    m_currentState = state;
}


void FSM::handleTransition( const std::string &event )
{
    std::string tmpEvent = event;
    Key_t key( m_currentState, event );
    std::map<Key_t, Data_t>::const_iterator it;

    // Find a transition
    it = m_transitions.find( key );

    // While the matching fails, try to match a more generic transition
    // For example, if "key:up:F" isn't a transition, "key:up" or "key" may be
    while( it == m_transitions.end() &&
           tmpEvent.rfind( ":", tmpEvent.size() ) != std::string::npos )
    {
        // Cut the last part
        tmpEvent = tmpEvent.substr( 0, tmpEvent.rfind( ":", tmpEvent.size() ) );

        key.second = tmpEvent;
        it = m_transitions.find( key );
    }

    // No transition was found
    if( it == m_transitions.end() )
    {
        return;
    }

    // Change state
    m_currentState = (*it).second.first;

    // Call the callback, if any
    CmdGeneric *pCmd = (*it).second.second;
    if( pCmd != NULL )
    {
        pCmd->execute();
    }
}
