/*****************************************************************************
 * evt_enter.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef EVT_ENTER_HPP
#define EVT_ENTER_HPP

#include "evt_input.hpp"


/// Mouse enter event
class EvtEnter: public EvtInput
{
public:
    EvtEnter( intf_thread_t *pIntf ): EvtInput( pIntf ) { }
    virtual ~EvtEnter() { }
    virtual const std::string getAsString() const { return "enter"; }
};


#endif
