// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * anchor.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef ANCHOR_HPP
#define ANCHOR_HPP

#include "skin_common.hpp"
#include "generic_layout.hpp"
#include "../utils/bezier.hpp"
#include "../utils/position.hpp"


/// Class for the windows anchors
class Anchor: public SkinObject
{
public:
    Anchor( intf_thread_t *pIntf, const Position &rPosition, int range,
            int priority, const Bezier &rCurve, GenericLayout &rLayout )
          : SkinObject( pIntf ), m_position( rPosition ), m_rCurve( rCurve ),
            m_range( range ), m_priority( priority ), m_rLayout( rLayout ) { }
    virtual ~Anchor() { }

    /**
     * Return true if the given anchor is hanged by this one
     * Two conditions are required:
     *  - the other anchor must be in position of anchoring (as defined
     *    by canHang())
     *  - the priority of the other anchor must be lower than this one's
     */
    bool isHanging( const Anchor &rOther ) const;

    /**
     * Return true if the other anchor, moved by the (xOffset, yOffset)
     * vector, is "hangable" by this one (i.e. if it is in its range of
     * action), else return false.
     * When the function returns true, the xOffset and yOffset parameters
     * are modified to indicate the position that the other anchor would
     * take if hanged by this one (this position is calculated to minimize
     * the difference with the old xOffset and yOffset, so that the window
     * doesn't "jump" in a strange way).
     */
    bool canHang( const Anchor &rOther, int &xOffset, int &yOffset ) const;

    // Indicate whether this anchor is reduced to a single point
    bool isPoint() const { return m_rCurve.getNbCtrlPoints() == 1; }

    // Getters
    const Position & getPosition() const { return m_position; }

    int getXPosAbs() const
    {
        return (m_position.getLeft() + m_rLayout.getLeft());
    }

    int getYPosAbs() const
    {
        return (m_position.getTop() + m_rLayout.getTop());
    }

private:
    /// Position in the layout
    Position m_position;

    /// Curve of the anchor
    const Bezier &m_rCurve;

    /// Range of action
    int m_range;

    /// Priority
    int m_priority;

    /// Parent layout
    GenericLayout &m_rLayout;
};


#endif
