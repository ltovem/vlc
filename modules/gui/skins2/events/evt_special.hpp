// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * evt_special.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef EVT_SPECIAL_HPP
#define EVT_SPECIAL_HPP

#include "evt_generic.hpp"


/// Class for non-genuine events
class EvtSpecial: public EvtGeneric
{
public:
    enum ActionType_t
    {
        kShow,
        kHide,
        kEnable,
        kDisable
    };

    EvtSpecial( intf_thread_t *pIntf, ActionType_t action )
              : EvtGeneric( pIntf ), m_action( action ) { }
    virtual ~EvtSpecial() { }
    virtual const std::string getAsString() const;

private:
    /// Type of action
    ActionType_t m_action;
};


#endif
