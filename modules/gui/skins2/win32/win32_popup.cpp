/*****************************************************************************
 * win32_popup.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifdef WIN32_SKINS

#include "win32_popup.hpp"
#include "win32_factory.hpp"

#ifndef TPM_NOANIMATION
const UINT TPM_NOANIMATION = 0x4000L;
#endif


Win32Popup::Win32Popup( intf_thread_t *pIntf, HWND hAssociatedWindow )
    : OSPopup( pIntf ), m_hWnd( hAssociatedWindow )
{
    // Create the popup menu
    m_hMenu = CreatePopupMenu();

    if( !m_hMenu )
    {
        msg_Err( getIntf(), "CreatePopupMenu failed" );
        return;
    }
}


Win32Popup::~Win32Popup()
{
    if( m_hMenu )
        DestroyMenu( m_hMenu );
}


void Win32Popup::show( int xPos, int yPos )
{
    TrackPopupMenuEx( m_hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON
                               | TPM_HORIZONTAL | TPM_NOANIMATION,
                      xPos, yPos, m_hWnd, NULL );
}


void Win32Popup::hide()
{
    SendMessage( m_hWnd, WM_CANCELMODE, 0, 0 );
}


void Win32Popup::addItem( const std::string &rLabel, int pos )
{
    MENUITEMINFO menuItem;
    menuItem.cbSize = sizeof( MENUITEMINFO );
//     menuItem.fMask = MIIM_FTYPE | MIIM_ID | MIIM_TYPE | MIIM_STRING;
//     menuItem.fType = MFT_STRING;
    menuItem.fMask = MIIM_ID | MIIM_STRING;
    menuItem.wID = pos;
    menuItem.dwTypeData = ToWide(rLabel.c_str());
    menuItem.cch = rLabel.size();

    InsertMenuItem( m_hMenu, findInsertionPoint( pos ), TRUE, &menuItem );
    free(menuItem.dwTypeData);
}


void Win32Popup::addSeparator( int pos )
{
    MENUITEMINFO sepItem;
    sepItem.cbSize = sizeof( MENUITEMINFO );
    sepItem.fMask = MIIM_FTYPE;
    sepItem.fType = MFT_SEPARATOR;

    InsertMenuItem( m_hMenu, findInsertionPoint( pos ), TRUE, &sepItem );
}


unsigned int Win32Popup::findInsertionPoint( unsigned int pos ) const
{
    // For this simple algorithm, we rely on the fact that in the final state
    // of the menu, the ID of each item is equal to its position in the menu
    int i = 0;
    while( i < GetMenuItemCount( m_hMenu ) &&
           GetMenuItemID( m_hMenu, i ) < pos )
    {
        i++;
    }
    return i;
}


#endif

