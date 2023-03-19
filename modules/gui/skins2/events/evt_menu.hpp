// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * evt_menu.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef EVT_MENU_HPP
#define EVT_MENU_HPP

#include "evt_generic.hpp"


/// Mouse move event
class EvtMenu: public EvtGeneric
{
public:
    EvtMenu( intf_thread_t *pIntf, int itemId )
           : EvtGeneric( pIntf ), m_itemId( itemId ) { }
    virtual ~EvtMenu() { }
    virtual const std::string getAsString() const { return "menu"; }

    int getItemId() const { return m_itemId; }

private:
    /// Coordinates of the mouse (absolute or relative)
    int m_itemId;
};


#endif
