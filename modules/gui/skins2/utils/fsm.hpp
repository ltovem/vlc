/*****************************************************************************
 * fsm.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef FSM_HPP
#define FSM_HPP

#include "../src/skin_common.hpp"
#include <string>
#include <map>
#include <set>

class EvtGeneric;
class CmdGeneric;


/// This class implements a Finite State Machine (FSM)
class FSM: public SkinObject
{
public:
    FSM( intf_thread_t *pIntf ): SkinObject( pIntf ) { }
    virtual ~FSM() { }

    /// Add a state to the machine
    void addState( const std::string &state );

    /// Add a transition to the machine
    void addTransition( const std::string &state1, const std::string &event,
                        const std::string &state2,
                        CmdGeneric *pCmd = NULL );

    /// Retrieve the current state
    const std::string &getState() const { return m_currentState; }

    /// Set the current state, without bothering about transitions
    void setState( const std::string &state );

    /// Find a transition from the current state with the input event,
    /// change the state, and call the associated callback (if any).
    void handleTransition( const std::string &event );

private:
    /// A Key_t contains the initial state of a transition, and a string
    /// characterizing an event (for example: "mouse:left:down:ctrl")
    typedef std::pair<std::string, std::string> Key_t;

    /// A Data_t contains the final state of a transition, and a callback
    /// to execute when the transition is applied
    typedef std::pair<std::string, CmdGeneric*> Data_t;

    /// Current state of the machine
    std::string m_currentState;

    /// Set containing the different states
    std::set<std::string> m_states;

    /// Map containing the different transitions between defined types
    /// It associates a final state (and potentially a callback)
    /// with a couple of the form: (currentState, triggerEvent)
    std::map<Key_t, Data_t> m_transitions;
};


#endif
