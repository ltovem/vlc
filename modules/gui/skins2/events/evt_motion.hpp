/*****************************************************************************
 * evt_motion.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef EVT_MOTION_HPP
#define EVT_MOTION_HPP

#include "evt_input.hpp"


/// Mouse move event
class EvtMotion: public EvtInput
{
public:
    EvtMotion( intf_thread_t *pIntf, int xPos, int yPos )
             : EvtInput( pIntf ), m_xPos( xPos ), m_yPos( yPos ) { }
    virtual ~EvtMotion() { }
    virtual const std::string getAsString() const { return "motion"; }

    // Getters
    int getXPos() const { return m_xPos; }
    int getYPos() const { return m_yPos; }

private:
    /// Coordinates of the mouse (absolute or relative)
    /**
     * The coordinates are absolute when the event is sent to the
     * GenericWindow, but are relative to the window when the event is
     * forwarded to the controls
     */
    int m_xPos, m_yPos;
};


#endif
