/*****************************************************************************
 * bezier.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef BEZIER_HPP
#define BEZIER_HPP

#include "../src/skin_common.hpp"
#include "pointer.hpp"
#include <vector>

#define MAX_BEZIER_POINT 1023


/// Class for Bezier curves
class Bezier: public SkinObject
{
public:
    /// Values to indicate which coordinate(s) must be checked to consider
    /// that two points are distinct
    enum Flag_t
    {
        kCoordsBoth,    // x or y must be different (default)
        kCoordsX,       // only x is different
        kCoordsY        // only y is different
    };

    Bezier( intf_thread_t *p_intf,
            const std::vector<float> &pAbscissas,
            const std::vector<float> &pOrdinates,
            Flag_t flag = kCoordsBoth );
    ~Bezier() { }

    /// Get the number of control points used to define the curve
    int getNbCtrlPoints() const { return m_nbCtrlPt; }

    /// Return the percentage (between 0 and 1) of the curve point nearest
    /// from (x, y)
    float getNearestPercent( int x, int y ) const;

    /// Return the distance of (x, y) to the curve, corrected
    /// by the (optional) given scale factors
    float getMinDist( int x, int y, float xScale = 1.0f,
                      float yScale = 1.0f ) const;

    /// Get the coordinates of the point at t percent of
    /// the curve (t must be between 0 and 1)
    void getPoint( float t, int &x, int &y ) const;

    /// Get the width (maximum abscissa) of the curve
    int getWidth() const;

    /// Get the height (maximum ordinate) of the curve
    int getHeight() const;

private:
    /// Number of control points
    int m_nbCtrlPt;
    /// vectors containing the coordinates of the control points
    std::vector<float> m_ptx;
    std::vector<float> m_pty;
    /// Vector containing precalculated factoriels
    std::vector<float> m_ft;

    /// Number of points (=pixels) used by the curve
    int m_nbPoints;
    /// Vectors with the coordinates of the different points of the curve
    std::vector<int> m_leftVect;
    std::vector<int> m_topVect;
    /// Vector with the percentages associated with the points of the curve
    std::vector<float> m_percVect;

    /// Return the index of the curve point that is the nearest from (x, y)
    int findNearestPoint( int x, int y ) const;
    /// Compute the coordinates of a point corresponding to a given
    /// percentage
    void computePoint( float t, int &x, int &y ) const;
    /// Helper function to compute a coefficient of the curve
    inline float computeCoeff( int i, int n, float t ) const;
    /// x^n
    static inline float power( float x, int n );
};


typedef CountedPtr<Bezier> BezierPtr;

#endif
