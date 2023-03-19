// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * win32_window.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef WIN32_WINDOW_HPP
#define WIN32_WINDOW_HPP

#include "../src/generic_window.hpp"
#include "../src/os_window.hpp"

#include <vlc_window.h>

#include <windows.h>
#include <ole2.h>   // LPDROPTARGET


/// Win32 implementation of OSWindow
class Win32Window: public OSWindow
{
public:
    Win32Window( intf_thread_t *pIntf, GenericWindow &rWindow,
                 HINSTANCE hInst, HWND hParentWindow,
                 bool dragDrop,
                 Win32Window *pParentWindow, GenericWindow::WindowType_t );
    virtual ~Win32Window();

    // Show the window
    virtual void show() const;

    // Hide the window
    virtual void hide() const;

    /// Move and resize the window
    virtual void moveResize( int left, int top, int width, int height ) const;

    /// Bring the window on top
    virtual void raise() const;

    /// Set the opacity of the window (0 = transparent, 255 = opaque)
    virtual void setOpacity( uint8_t value ) const;

    /// Toggle the window on top
    virtual void toggleOnTop( bool onTop ) const;

    /// Set the window handler
    void setOSHandle( vlc_window_t *pWnd ) const {
        pWnd->type = VLC_WINDOW_TYPE_HWND;
        pWnd->info.has_double_click = true;
        pWnd->handle.hwnd = m_hWnd;
    }

    /// Getter for the window handle
    HWND getHandle() const { return m_hWnd; }

    /// reparent the window
    void reparent( OSWindow* parent, int x, int y, int w, int h );

    /// invalidate a window surface
    bool invalidateRect( int x, int y, int w, int h ) const;

private:
    /// Window handle
    HWND m_hWnd;
    /// Window parent's handle
    HWND m_hWnd_parent;
    /// Indicates whether the window handles drag&drop events
    bool m_dragDrop;
    /// Drop target
    LPDROPTARGET m_pDropTarget;
    /// Indicates whether the window is layered
    mutable bool m_isLayered;
    /// window type
    GenericWindow::WindowType_t m_type;

};


#endif
