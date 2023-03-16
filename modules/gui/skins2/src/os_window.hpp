// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * os_window.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef OS_WINDOW_HPP
#define OS_WINDOW_HPP

#include "skin_common.hpp"

class GenericWindow;
class OSGraphics;


/// OS specific delegate class for GenericWindow
class OSWindow: public SkinObject
{
public:
    virtual ~OSWindow() { }

    // Show the window
    virtual void show() const = 0;

    // Hide the window
    virtual void hide() const = 0;

    /// Move and resize the window
    virtual void moveResize( int left, int top,
                             int width, int height ) const = 0;

    /// Bring the window on top
    virtual void raise() const = 0;

    /// Set the opacity of the window (0 = transparent, 255 = opaque)
    virtual void setOpacity( uint8_t value ) const = 0;

    /// Toggle the window on top
    virtual void toggleOnTop( bool onTop ) const = 0;

    /// getter for handler
    virtual void setOSHandle( struct vlc_window *pWnd ) const = 0;

    /// reparent the window
    virtual void reparent( OSWindow *window,
                           int x, int y, int w, int h ) = 0;

    /// updateWindow (tell the OS we need to update the window)
    virtual bool invalidateRect( int x, int y, int w, int h ) const = 0;

protected:
    OSWindow( intf_thread_t *pIntf ): SkinObject( pIntf ) { }
};


#endif
