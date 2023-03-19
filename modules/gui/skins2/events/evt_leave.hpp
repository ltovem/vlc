// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * evt_leave.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef EVT_LEAVE_HPP
#define EVT_LEAVE_HPP

#include "evt_input.hpp"


/// Mouse leave event
class EvtLeave: public EvtInput
{
public:
    EvtLeave( intf_thread_t *pIntf ): EvtInput( pIntf ) { }
    virtual ~EvtLeave() { }
    virtual const std::string getAsString() const { return "leave"; }
};


#endif
