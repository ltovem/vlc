/*****************************************************************************
 * win32_tooltip.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef WIN32_TOOLTIP_HPP
#define WIN32_TOOLTIP_HPP

#include "../src/os_tooltip.hpp"
#include <windows.h>


/// Win32 implementation of OSTooltip
class Win32Tooltip: public OSTooltip
{
public:
    Win32Tooltip( intf_thread_t *pIntf, HINSTANCE hInst, HWND hParentWindow );

    virtual ~Win32Tooltip();

    /// Show the tooltip
    virtual void show( int left, int top, OSGraphics &rText );

    /// Hide the tooltip
    virtual void hide();

private:
    /// Window ID
    HWND m_hWnd;
};


#endif
