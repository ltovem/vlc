/*****************************************************************************
 * fsc_window.hpp
 *****************************************************************************
 * Copyright (C) 2010 the VideoLAN team
 *
 * Author: Erwan Tulou      <erwan10 At videolan Dot Org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef FSC_WINDOW_HPP
#define FSC_WINDOW_HPP

#include "../src/top_window.hpp"
#include "../commands/cmd_generic.hpp"

class OSTimer;

/// Class for the fullscreen controller
class FscWindow: public TopWindow
{
public:
    FscWindow( intf_thread_t *pIntf, int left, int top,
                              WindowManager &rWindowManager,
                              bool dragDrop, bool playOnDrop, bool visible );

    virtual ~FscWindow();

    virtual void processEvent( EvtLeave &rEvtLeave );
    virtual void processEvent( EvtMotion &rEvtMotion );

    /// Method called when fullscreen indicator changes
    virtual void onUpdate( Subject<VarBool> &rVariable , void* );

    /// Action when window is shown
    virtual void innerShow();

    /// Action when window is hidden
    virtual void innerHide();

    /// Action when mouse moved
    virtual void onMouseMoved();

    /// Action for each transition of fading out
    virtual void onTimerExpired();

    /// Relocate fsc into new area
    virtual void moveTo( int x, int y, int width, int height );

private:
    /// Timer for fsc fading-out
    OSTimer *m_pTimer;
    /// number of transitions when fading out
    int m_count;
    /// opacity set by user
    int m_opacity;
    /// delay set by user
    int m_delay;
    /// activation set by user
    bool m_enabled;

    /// Callback for the timer
    DEFINE_CALLBACK( FscWindow, FscHide )
};


#endif
