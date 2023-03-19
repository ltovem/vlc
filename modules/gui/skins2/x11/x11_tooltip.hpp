// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * x11_tooltip.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef X11_TOOLTIP_HPP
#define X11_TOOLTIP_HPP

#include <X11/Xlib.h>

#include "../src/os_tooltip.hpp"

class X11Display;


/// X11 implementation of OSTooltip
class X11Tooltip: public OSTooltip
{
public:
    X11Tooltip( intf_thread_t *pIntf, X11Display &rDisplay );

    virtual ~X11Tooltip();

    /// Show the tooltip
    virtual void show( int left, int top, OSGraphics &rText );

    /// Hide the tooltip
    virtual void hide();

private:
    /// X11 display
    X11Display &m_rDisplay;
    /// Window ID
    Window m_wnd;
};


#endif
