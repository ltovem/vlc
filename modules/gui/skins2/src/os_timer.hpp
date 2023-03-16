/*****************************************************************************
 * os_timer.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef OS_TIMER_HPP
#define OS_TIMER_HPP

#include "skin_common.hpp"


// Base class for OS-specific timers
class OSTimer: public SkinObject
{
public:
    virtual ~OSTimer() { }

    /// (Re)start the timer with the given delay (in ms). If oneShot is
    /// true, stop it after the first execution of the callback.
    virtual void start( int delay, bool oneShot ) = 0;

    /// Stop the timer
    virtual void stop() = 0;

protected:
    OSTimer( intf_thread_t *pIntf ): SkinObject( pIntf ) { }
};


#endif
