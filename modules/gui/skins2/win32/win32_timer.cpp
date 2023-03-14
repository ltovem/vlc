/*****************************************************************************
 * win32_timer.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifdef WIN32_SKINS

#include "win32_timer.hpp"
#include "../commands/cmd_generic.hpp"


void CALLBACK CallbackTimer( HWND hwnd, UINT uMsg,
                             UINT_PTR idEvent, DWORD dwTime )
{
    (void)hwnd; (void)uMsg; (void)dwTime;
    Win32Timer *pTimer = (Win32Timer*)idEvent;
    pTimer->execute();
}


Win32Timer::Win32Timer( intf_thread_t *pIntf, CmdGeneric &rCmd, HWND hWnd ):
    OSTimer( pIntf ), m_rCommand( rCmd ), m_hWnd( hWnd )
{
}


Win32Timer::~Win32Timer()
{
    stop();

    // discard possible WM_TIMER messages for this timer
    // already in the message queue and not yet dispatched
    MSG msg;
    while( !PeekMessage( &msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE ) )
    {
        if( (Win32Timer*)msg.wParam != this )
            PostMessage( m_hWnd, WM_TIMER, msg.wParam, msg.lParam );
    }
}


void Win32Timer::start( int delay, bool oneShot )
{
    m_interval = delay;
    m_oneShot = oneShot;
    SetTimer( m_hWnd, (UINT_PTR)this, m_interval, (TIMERPROC)CallbackTimer );
}


void Win32Timer::stop()
{
    KillTimer( m_hWnd, (UINT_PTR)this );
}


void Win32Timer::execute()
{
    // Execute the callback
    m_rCommand.execute();

    // Stop the timer if requested
    if( m_oneShot )
        stop();
}

#endif
