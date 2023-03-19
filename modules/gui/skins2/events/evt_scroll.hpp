// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * evt_scroll.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef EVT_SCROLL_HPP
#define EVT_SCROLL_HPP

#include "evt_input.hpp"


/// Class for mouse scroll events
class EvtScroll: public EvtInput
{
public:
    enum Direction_t
    {
        kUp,
        kDown
    };

    EvtScroll( intf_thread_t *pIntf, int xPos, int yPos,
               Direction_t direction, int mod = kModNone )
             : EvtInput( pIntf, mod ), m_xPos( xPos ), m_yPos( yPos ),
               m_direction( direction ) { }
    virtual ~EvtScroll() { }
    virtual const std::string getAsString() const;

    // Return the event coordinates
    int getXPos() const { return m_xPos; }
    int getYPos() const { return m_yPos; }

    // Return the direction
    Direction_t getDirection() const { return m_direction; }

private:
    /// Coordinates of the mouse relative to the window
    int m_xPos, m_yPos;
    /// Scroll direction
    Direction_t m_direction;
};


#endif
