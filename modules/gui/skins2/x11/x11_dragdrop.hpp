// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * x11_dragdrop.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef X11_DRAGDROP_HPP
#define X11_DRAGDROP_HPP

#include <X11/Xlib.h>
#include "../src/skin_common.hpp"
#include "../src/generic_window.hpp"

class X11Display;


class X11DragDrop: public SkinObject
{
public:
    typedef long ldata_t[5];

    X11DragDrop( intf_thread_t *pIntf, X11Display &rDisplay, Window win,
                 bool playOnDrop, GenericWindow *pWin );
    virtual ~X11DragDrop() { }

    void dndEnter( ldata_t data );
    void dndPosition( ldata_t data );
    void dndLeave( ldata_t data );
    void dndDrop( ldata_t data );
    void dndSelectionNotify( );

private:
    /// X11 display
    X11Display &m_rDisplay;
    /// Window ID
    Window m_wnd;
    /// Indicates whether the file(s) must be played immediately
    bool m_playOnDrop;
    /// Target type
    Atom m_target;
    /// Generic Window
    GenericWindow *m_pWin;
    /// (x,y) mouse coordinates
    int m_xPos;
    int m_yPos;
};


#endif
