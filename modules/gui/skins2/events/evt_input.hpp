/*****************************************************************************
 * evt_input.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef EVT_INPUT_HPP
#define EVT_INPUT_HPP

#include "evt_generic.hpp"

/// Base class for mouse and key events
class EvtInput: public EvtGeneric
{
public:
    virtual ~EvtInput() { }

    /// Masks for modifier keys
    static const int
        kModNone, kModAlt, kModShift, kModCtrl, kModMeta, kModCmd;

    /// Get the modifiers
    int getMod() const { return m_mod; }

protected:
    EvtInput( intf_thread_t *pIntf, int mod = kModNone );

    /// Add the modifier to the event string
    void addModifier( std::string &rEvtString ) const;

private:
    /// Modifiers (special key(s) pressed during the mouse event)
    int m_mod;
};

#endif
