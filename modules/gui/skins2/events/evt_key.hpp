// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * evt_key.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef EVT_KEY_HPP
#define EVT_KEY_HPP

#include "evt_input.hpp"


/// Class for keyboard events
class EvtKey: public EvtInput
{
public:
    enum ActionType_t
    {
        kDown,
        kUp
    };

    EvtKey( intf_thread_t *I, int key, ActionType_t actn, int mod = kModNone )
          : EvtInput( I, mod ), m_key( key ), m_action( actn ) { }
    virtual ~EvtKey() { }
    virtual const std::string getAsString() const;

    int getKey() const { return m_key; }
    int getModKey() const { return m_key | getMod(); }

    ActionType_t getKeyState() const { return m_action; }

private:
    /// The concerned key, stored according to the '#define's in vlc_actions.h
    /// but without the modifiers (which are stored in EvtInput)
    int m_key;
    /// Type of action
    ActionType_t m_action;
};


#endif
