// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * evt_generic.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef EVT_GENERIC_HPP
#define EVT_GENERIC_HPP

#include "../src/skin_common.hpp"
#include <string>


/// Base class for OS events
class EvtGeneric: public SkinObject
{
public:
    virtual ~EvtGeneric() { }

    /// Return the type of the event
    virtual const std::string getAsString() const = 0;

protected:
    EvtGeneric( intf_thread_t *pIntf ): SkinObject( pIntf ) { }
};


#endif
