/*****************************************************************************
 * x11_popup.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef X11_POPUP_HPP
#define X11_POPUP_HPP

#include "../src/os_popup.hpp"

class X11Display;

/// X11 implementation of OSPopup
class X11Popup: public OSPopup
{
public:
    X11Popup( intf_thread_t *pIntf, X11Display & );

    virtual ~X11Popup();

    /// Show the popup menu at the given (absolute) coordinates
    virtual void show( int xPos, int yPos );

    /// Hide the popup menu
    virtual void hide();

    /// Append a new menu item with the given label to the popup menu
    virtual void addItem( const std::string &rLabel, int pos );

    /// Create a dummy menu item to separate sections
    virtual void addSeparator( int pos );

    /// Return the position of the item identified by the given id
    virtual int getPosFromId( int id ) const;
};


#endif
