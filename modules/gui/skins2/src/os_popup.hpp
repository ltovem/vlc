/*****************************************************************************
 * os_popup.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef OS_POPUP_HPP
#define OS_POPUP_HPP

#include "skin_common.hpp"
#include <string>

class OSGraphics;


/// Base class for OS specific Popup Windows
class OSPopup: public SkinObject
{
public:
    virtual ~OSPopup() { }

    /// Show the popup menu at the given (absolute) coordinates
    virtual void show( int xPos, int yPos ) = 0;

    /// Hide the popup menu
    virtual void hide() = 0;

    /// Append a new menu item with the given label to the popup menu
    virtual void addItem( const std::string &rLabel, int pos ) = 0;

    /// Create a dummy menu item to separate sections
    virtual void addSeparator( int pos ) = 0;

    /// Return the position of the item identified by the given id
    virtual int getPosFromId( int id ) const = 0;

protected:
    OSPopup( intf_thread_t *pIntf ): SkinObject( pIntf ) { }
};

#endif
