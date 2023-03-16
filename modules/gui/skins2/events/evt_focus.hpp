/*****************************************************************************
 * evt_focus.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef EVT_FOCUS_HPP
#define EVT_FOCUS_HPP

#include "evt_generic.hpp"


/// Focus change event
class EvtFocus: public EvtGeneric
{
public:
    EvtFocus( intf_thread_t *pIntf, bool focus )
            : EvtGeneric( pIntf ), m_focus( focus ) { }
    virtual ~EvtFocus() { }

    virtual const std::string getAsString() const
    {
        return ( m_focus ? "focus:in" : "focus:out" );
    }

private:
    /// true for a focus in, and false for a focus out
    bool m_focus;
};


#endif
