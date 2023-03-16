// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * os2_tooltip.hpp
 *****************************************************************************
 * Copyright (C) 2003, 2013 the VideoLAN team
 *
 * Authors: Cyril Deguet      <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *          KO Myung-Hun      <komh@chollian.net>
 *****************************************************************************/

#ifndef OS2_TOOLTIP_HPP
#define OS2_TOOLTIP_HPP

#include "../src/os_tooltip.hpp"


/// OS2 implementation of OSTooltip
class OS2Tooltip: public OSTooltip
{
public:
    OS2Tooltip( intf_thread_t *pIntf, HMODULE hInst, HWND hParentWindow );

    virtual ~OS2Tooltip();

    /// Show the tooltip
    virtual void show( int left, int top, OSGraphics &rText );

    /// Hide the tooltip
    virtual void hide();

private:
    /// Window ID
    HWND m_hWnd;

    /// Client window ID
    HWND m_hWndClient;
};


#endif
