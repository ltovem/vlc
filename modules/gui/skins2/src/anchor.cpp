// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * anchor.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#include "anchor.hpp"


bool Anchor::isHanging( const Anchor &rOther ) const
{
    if( m_priority <= rOther.m_priority )
        return false;

    // Compute delta coordinates between anchors, since the Bezier class
    // uses coordinates relative to (0;0)
    int deltaX = getXPosAbs() - rOther.getXPosAbs();
    int deltaY = getYPosAbs() - rOther.getYPosAbs();

    // One of the anchors (at least) must be a point, else it has no meaning
    return (isPoint() && rOther.m_rCurve.getMinDist( deltaX, deltaY ) == 0) ||
           (rOther.isPoint() && m_rCurve.getMinDist( -deltaX, -deltaY ) == 0);
}


bool Anchor::canHang( const Anchor &rOther, int &xOffset, int &yOffset ) const
{
    int deltaX = getXPosAbs() - (rOther.getXPosAbs() + xOffset);
    int deltaY = getYPosAbs() - (rOther.getYPosAbs() + yOffset);

    // One of the anchors (at least) must be a point, else it has no meaning
    if( (isPoint() && rOther.m_rCurve.getMinDist( deltaX, deltaY ) < m_range) )
    {
        // Compute the coordinates of the nearest point of the curve
        int xx, yy;
        float p = rOther.m_rCurve.getNearestPercent( deltaX, deltaY );
        rOther.m_rCurve.getPoint( p, xx, yy );

        xOffset = getXPosAbs() - (rOther.getXPosAbs() + xx);
        yOffset = getYPosAbs() - (rOther.getYPosAbs() + yy);
        return true;
    }
    else if( (rOther.isPoint() &&
              m_rCurve.getMinDist( -deltaX, -deltaY ) < m_range) )
    {
        // Compute the coordinates of the nearest point of the curve
        int xx, yy;
        float p = m_rCurve.getNearestPercent( -deltaX, -deltaY );
        m_rCurve.getPoint( p, xx, yy );

        xOffset = (getXPosAbs() + xx) - rOther.getXPosAbs();
        yOffset = (getYPosAbs() + yy) - rOther.getYPosAbs();
        return true;
    }
    else
    {
        return false;
    }
}
