/*****************************************************************************
 * os2_timer.hpp
 *****************************************************************************
 * Copyright (C) 2003, 2013 the VideoLAN team
 *
 * Authors: Cyril Deguet      <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *          KO Myung-Hun      <komh@chollian.net>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef OS2_TIMER_HPP
#define OS2_TIMER_HPP

#include "../src/os_timer.hpp"

class CmdGeneric;

// OS2 specific timer
class OS2Timer: public OSTimer
{
public:
    OS2Timer( intf_thread_t *pIntf, CmdGeneric &rCmd, HWND hWnd );
    virtual ~OS2Timer();

    /// (Re)start the timer with the given delay (in ms). If oneShot is
    /// true, stop it after the first execution of the callback.
    virtual void start( int delay, bool oneShot );

    /// Stop the timer
    virtual void stop();

    /// Execute the callback
    void execute();

private:
    /// Command to execute
    CmdGeneric &m_rCommand;

    /// Delay between two execute
    vlc_tick_t m_interval;

    /// Flag to tell whether the timer must be stopped after the
    /// first execution
    bool m_oneShot;

    /// Handle of the window to which the timer will be attached
    HWND m_hWnd;

    /// Handle of the timer window
    HWND m_hwndTimer;
};


#endif
