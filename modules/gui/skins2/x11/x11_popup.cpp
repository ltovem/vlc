// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * x11_popup.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifdef X11_SKINS

#include "x11_popup.hpp"


X11Popup::X11Popup( intf_thread_t *pIntf, X11Display &rDisplay )
    : OSPopup( pIntf )
{
    // TODO
    (void)rDisplay;
}


X11Popup::~X11Popup()
{
    // TODO
}


void X11Popup::show( int xPos, int yPos )
{
    // TODO
    (void)xPos; (void)yPos;
}


void X11Popup::hide()
{
    // TODO
}


void X11Popup::addItem( const std::string &rLabel, int pos )
{
    // TODO
    (void)rLabel; (void)pos;
}


void X11Popup::addSeparator( int pos )
{
    // TODO
    (void)pos;
}


int X11Popup::getPosFromId( int id ) const
{
    // TODO
    (void)id;
    return 0;
}


#endif

