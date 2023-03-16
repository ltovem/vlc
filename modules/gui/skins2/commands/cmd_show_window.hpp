/*****************************************************************************
 * cmd_show_window.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef CMD_SHOW_WINDOW_HPP
#define CMD_SHOW_WINDOW_HPP

#include "cmd_generic.hpp"
#include "../src/os_factory.hpp"
#include "../src/top_window.hpp"
#include "../src/window_manager.hpp"
#include "../src/popup.hpp"


/// Command to show a window
class CmdShowWindow: public CmdGeneric
{
public:
    CmdShowWindow( intf_thread_t *pIntf, WindowManager &rWinManager,
                   TopWindow &rWin ):
        CmdGeneric( pIntf ), m_rWinManager( rWinManager ), m_rWin( rWin ) { }
    virtual ~CmdShowWindow() { }
    virtual void execute() { m_rWinManager.show( m_rWin ); }
    virtual std::string getType() const { return "show window"; }

private:
    /// Reference to the window manager
    WindowManager &m_rWinManager;
    /// Reference to the window
    TopWindow &m_rWin;
};


/// Command to hide a window
class CmdHideWindow: public CmdGeneric
{
public:
    CmdHideWindow( intf_thread_t *pIntf, WindowManager &rWinManager,
                   TopWindow &rWin ):
        CmdGeneric( pIntf ), m_rWinManager( rWinManager ), m_rWin( rWin ) { }
    virtual ~CmdHideWindow() { }
    virtual void execute() { m_rWinManager.hide( m_rWin ); }
    virtual std::string getType() const { return "hide window"; }

private:
    /// Reference to the window manager
    WindowManager &m_rWinManager;
    /// Reference to the window
    TopWindow &m_rWin;
};


/// Command to raise all windows
class CmdRaiseAll: public CmdGeneric
{
public:
    CmdRaiseAll( intf_thread_t *pIntf, WindowManager &rWinManager ):
        CmdGeneric( pIntf ), m_rWinManager( rWinManager ) { }
    virtual ~CmdRaiseAll() { }
    virtual void execute() { m_rWinManager.raiseAll(); }
    virtual std::string getType() const { return "raise all windows"; }

private:
    /// Reference to the window manager
    WindowManager &m_rWinManager;
};


/// Command to show a popup menu
class CmdShowPopup: public CmdGeneric
{
public:
    CmdShowPopup( intf_thread_t *pIntf, Popup &rPopup ):
        CmdGeneric( pIntf ), m_rPopup( rPopup ) { }
    virtual ~CmdShowPopup() { }

    virtual void execute()
    {
        int x, y;
        OSFactory::instance( getIntf() )->getMousePos( x, y );
        m_rPopup.show( x, y );
    }

    virtual std::string getType() const { return "show popup"; }

private:
    /// Reference to the popup
    Popup &m_rPopup;
};


#endif
