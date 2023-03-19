// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * x11_timer.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef X11_TIMER_HPP
#define X11_TIMER_HPP

#include "../src/os_timer.hpp"

#include <list>

// Forward declaration
class X11TimerLoop;
class CmdGeneric;


// X11 specific timer
class X11Timer: public OSTimer
{
public:
    X11Timer( intf_thread_t *pIntf, CmdGeneric &rCmd );
    virtual ~X11Timer();

    /// (Re)start the timer with the given delay (in ms). If oneShot is
    /// true, stop it after the first execution of the callback.
    virtual void start( int delay, bool oneShot );

    /// Stop the timer
    virtual void stop();

    vlc_tick_t getNextDate() const;

    /// Execute the callback.
    /// Returns false if the timer must be removed after
    bool execute();

private:
    /// Command to execute
    CmdGeneric &m_rCommand;
    /// Timer loop
    X11TimerLoop *m_pTimerLoop;
    /// Delay between two execute
    vlc_tick_t m_interval;
    /// Next date at which the timer must be executed
    vlc_tick_t m_nextDate;
    /// Flag to tell if the timer must be stopped after the first execution
    bool m_oneShot;
};


/// Class to manage a set of timers
class X11TimerLoop: public SkinObject
{
public:
    /// Create the timer loop with the communication number of the X11
    /// display
    X11TimerLoop( intf_thread_t *pIntf, int connectionNumber );
    virtual ~X11TimerLoop();

    /// Add a timer in the manager
    void addTimer( X11Timer &rTimer );

    /// Remove a timer from the manager
    void removeTimer( X11Timer &rTimer );

    /// Wait for the next timer and execute it
    void waitNextTimer();

private:
    /// Connection number of the X11 display
    int m_connectionNumber;
    /// List of timers
    std::list<X11Timer*> m_timers;

    /// Sleep for delay milliseconds, unless an X11 event is received.
    /// Returns true if the sleep has been interrupted.
    bool sleep( int delay );
};


#endif
