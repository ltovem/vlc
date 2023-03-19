// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * evt_refresh.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef EVT_REFRESH_HPP
#define EVT_REFRESH_HPP

#include "evt_generic.hpp"


/// Refresh window event
class EvtRefresh: public EvtGeneric
{
public:
    /// Constructor with the coordinates of the area to refresh
    EvtRefresh( intf_thread_t *pIntf, int xStart, int yStart,
                                      int width, int height )
              : EvtGeneric( pIntf ), m_xStart( xStart ), m_yStart( yStart ),
                                     m_width( width ), m_height( height ) { }

    virtual ~EvtRefresh() { }
    virtual const std::string getAsString() const { return "refresh"; }

    /// Getters
    int getXStart() const { return m_xStart; }
    int getYStart() const { return m_yStart; }
    int getWidth()  const { return m_width; }
    int getHeight() const { return m_height; }

private:
    /// Coordinates and size of the area to refresh
    int m_xStart, m_yStart, m_width, m_height;
};


#endif
