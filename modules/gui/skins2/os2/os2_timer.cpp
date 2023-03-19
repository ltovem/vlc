// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * os2_timer.cpp
 *****************************************************************************
 * Copyright (C) 2003, 2013 the VideoLAN team
 *
 * Authors: Cyril Deguet      <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *          KO Myung-Hun      <komh@chollian.net>
 *****************************************************************************/

#ifdef OS2_SKINS

#include "os2_timer.hpp"
#include "../commands/cmd_generic.hpp"

#define OS2_TIMER_CLASS "OS2Timer Window Class"

#define ID_TIMER 0x100

static MRESULT EXPENTRY TimerWindowProc( HWND hwnd, ULONG msg,
                                         MPARAM mp1, MPARAM mp2 )
{
    if( msg == WM_TIMER )
    {
        OS2Timer *pTimer = ( OS2Timer * )WinQueryWindowPtr( hwnd, 0 );

        pTimer->execute();

        return 0;
    }

    return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}


OS2Timer::OS2Timer( intf_thread_t *pIntf, CmdGeneric &rCmd, HWND hWnd ):
    OSTimer( pIntf ), m_rCommand( rCmd ), m_hWnd( hWnd ), m_hwndTimer( 0 )
{
    WinRegisterClass( 0, OS2_TIMER_CLASS, TimerWindowProc, 0,
                      sizeof( PVOID ));
}


OS2Timer::~OS2Timer()
{
    stop();
}


void OS2Timer::start( int delay, bool oneShot )
{
    m_interval = delay;
    m_oneShot = oneShot;

    m_hwndTimer = WinCreateWindow( HWND_OBJECT, OS2_TIMER_CLASS,
                                   "", 0, 0, 0, 0, 0, NULLHANDLE,
                                   HWND_TOP, 0, NULL, NULL );

    WinSetWindowPtr( m_hwndTimer, 0, this );

    WinStartTimer( 0, m_hwndTimer, ID_TIMER, m_interval );
}


void OS2Timer::stop()
{
    if( !m_hwndTimer )
        return;

    WinStopTimer( 0, m_hwndTimer, ID_TIMER );

    WinDestroyWindow( m_hwndTimer );

    m_hwndTimer = NULLHANDLE;
}


void OS2Timer::execute()
{
    // Execute the callback
    m_rCommand.execute();

    // Stop the timer if requested
    if( m_oneShot )
        stop();
}

#endif
