// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * os2_loop.hpp
 *****************************************************************************
 * Copyright (C) 2003, 2013 the VideoLAN team
 *
 * Authors: Cyril Deguet      <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *          KO Myung-Hun      <komh@chollian.net>
 *****************************************************************************/

#ifndef OS2_LOOP_HPP
#define OS2_LOOP_HPP

#include "../events/evt_mouse.hpp"
#include "../src/os_loop.hpp"
#include <map>


class GenericWindow;

/// Main event loop for OS2
class OS2Loop: public OSLoop
{
public:
    /// Get the instance of OS2Loop
    static OSLoop *instance( intf_thread_t *pIntf );

    /// Destroy the instance of OS2Loop
    static void destroy( intf_thread_t *pIntf );

    OS2Loop( intf_thread_t *pIntf );

    /// Enter the event loop
    virtual void run();

    /// Exit the main loop
    virtual void exit();

    /// called by the window procedure callback
    virtual MRESULT EXPENTRY processEvent( HWND hwnd, ULONG msg,
                                           MPARAM mp1, MPARAM mp2 );

private:
    /// Map associating special (i.e. non ascii) virtual key codes with
    /// internal vlc key codes
    std::map<int, int> virtKeyToVlcKey;

    /// Helper function to find the modifier in a Windows message
    int getMod( MPARAM mp ) const;
};

#endif
