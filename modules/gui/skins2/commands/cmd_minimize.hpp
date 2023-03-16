/*****************************************************************************
 * cmd_minimize.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Mohammed Adnène Trojette     <adn@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef CMD_MINIMIZE_HPP
#define CMD_MINIMIZE_HPP

#include "cmd_generic.hpp"

class WindowManager;
class TopWindow;


DEFINE_COMMAND(Minimize, "minimize" )
DEFINE_COMMAND(Restore, "restore" )

/// Command to maximize a window
class CmdMaximize: public CmdGeneric
{
public:
    /// Maximize the given layout
    CmdMaximize( intf_thread_t *pIntf, WindowManager &rWindowManager,
                 TopWindow &rWindow );
    virtual ~CmdMaximize() { }
    virtual void execute();
    virtual std::string getType() const { return "maximize"; }

private:
    WindowManager &m_rWindowManager;
    TopWindow &m_rWindow;
};


/// Command to unmaximize a window
class CmdUnmaximize: public CmdGeneric
{
public:
    /// Unmaximize the given layout
    CmdUnmaximize( intf_thread_t *pIntf, WindowManager &rWindowManager,
                 TopWindow &rWindow );
    virtual ~CmdUnmaximize() { }
    virtual void execute();
    virtual std::string getType() const { return "unmaximize"; }

private:
    WindowManager &m_rWindowManager;
    TopWindow &m_rWindow;
};


DEFINE_COMMAND( AddInTray,         "add in tray" )
DEFINE_COMMAND( RemoveFromTray,    "remove from tray" )
DEFINE_COMMAND( AddInTaskBar,      "add in taskbar" )
DEFINE_COMMAND( RemoveFromTaskBar, "remove from taskbar" )

#endif
