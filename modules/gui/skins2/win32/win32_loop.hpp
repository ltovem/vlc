// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * win32_loop.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef WIN32_LOOP_HPP
#define WIN32_LOOP_HPP

#include "../events/evt_mouse.hpp"
#include "../src/os_loop.hpp"
#include <map>


class GenericWindow;

/// Main event loop for Win32
class Win32Loop: public OSLoop
{
public:
    Win32Loop( intf_thread_t *pIntf );

    /// Get the instance of Win32Loop
    static OSLoop *instance( intf_thread_t *pIntf );

    /// Destroy the instance of Win32Loop
    static void destroy( intf_thread_t *pIntf );

    /// Enter the event loop
    virtual void run();

    /// Exit the main loop
    virtual void exit();

    /// called by the window procedure callback
    virtual LRESULT CALLBACK processEvent( HWND hwnd, UINT msg,
                                           WPARAM wParam, LPARAM lParam );

private:

    /// Map associating special (i.e. non ascii) virtual key codes with
    /// internal vlc key codes
    std::map<int, int> virtKeyToVlcKey;

    /// Helper function to find the modifier in a Windows message
    int getMod( WPARAM wParam ) const;
};

#endif
