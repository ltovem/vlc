/*****************************************************************************
 * os2_tooltip.cpp
 *****************************************************************************
 * Copyright (C) 2003, 2013 the VideoLAN team
 *
 * Authors: Cyril Deguet      <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *          KO Myung-Hun      <komh@chollian.net>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifdef OS2_SKINS

#include "os2_tooltip.hpp"
#include "os2_graphics.hpp"
#include "../src/generic_window.hpp"

OS2Tooltip::OS2Tooltip( intf_thread_t *pIntf, HMODULE hInst,
                        HWND hParentWindow ):
    OSTooltip( pIntf )
{
    // Create the window
    ULONG flFrame = FCF_SHELLPOSITION;
    m_hWnd = WinCreateStdWindow( HWND_DESKTOP,
                                 WS_DISABLED,
                                 &flFrame,
                                 "SkinWindowClass",
                                 "tooltip",
                                 0,
                                 hInst,
                                 0,
                                 &m_hWndClient );

    if( !m_hWnd )
    {
        msg_Err( getIntf(), "createWindow failed" );
    }
}


OS2Tooltip::~OS2Tooltip()
{
    if( m_hWnd )
        WinDestroyWindow( m_hWnd );
}


void OS2Tooltip::show( int left, int top, OSGraphics &rText )
{
    // Source drawable
    HPS hpsSrc = ((OS2Graphics&)rText).getPS();
    int width = rText.getWidth();
    int height = rText.getHeight();

    RECTL rclParent;

    WinQueryWindowRect( WinQueryWindow( m_hWnd, QW_PARENT ), &rclParent );

    // Find bottom and invert it
    int bottom = ( rclParent.yTop - 1 ) - ( top + height - 1 );

    // Set the window on top, resize it, and show it
    WinSetWindowPos( m_hWnd, HWND_TOP, left, bottom, width, height,
                     SWP_ZORDER | SWP_MOVE | SWP_SIZE | SWP_SHOW );

    HPS hps = WinGetPS( m_hWnd );
    POINTL aptl[] = {{ 0, 0 }, { width, height }, { 0, 0 }};
    GpiBitBlt( hps, hpsSrc, 3, aptl, ROP_SRCCOPY, BBO_IGNORE );
    WinReleasePS( hps );
}


void OS2Tooltip::hide()
{
    WinShowWindow( m_hWnd, FALSE );
}


#endif
