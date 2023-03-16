/*****************************************************************************
 * os_graphics.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef OS_GRAPHICS_HPP
#define OS_GRAPHICS_HPP

#include "skin_common.hpp"
#include "../utils/position.hpp"
#include "../utils/pointer.hpp"

class GenericBitmap;
class OSWindow;


/// OS specific graphics class
class OSGraphics: public SkinObject, public Box
{
public:
    virtual ~OSGraphics() { }

    /// Clear the graphics
    virtual void clear( int xDest = 0, int yDest = 0,
                        int width = -1, int height = -1) = 0;

    /// Draw another graphics on this one
    virtual void drawGraphics( const OSGraphics &rGraphics, int xSrc = 0,
                               int ySrc = 0, int xDest = 0, int yDest = 0,
                               int width = -1, int height = -1 ) = 0;

    /// Render a bitmap on this graphics
    virtual void drawBitmap( const GenericBitmap &rBitmap, int xSrc = 0,
                             int ySrc = 0, int xDest = 0, int yDest = 0,
                             int width = -1, int height = -1,
                             bool blend = false) = 0;

    /// Draw a filled rectangle on the graphics (color is #RRGGBB)
    virtual void fillRect( int left, int top, int width, int height,
                           uint32_t color ) = 0;

    /// Draw an empty rectangle on the graphics (color is #RRGGBB)
    virtual void drawRect( int left, int top, int width, int height,
                           uint32_t color ) = 0;


    /// Set the shape of a window with the mask of this graphics.
    virtual void applyMaskToWindow( OSWindow &rWindow ) = 0;

    /// Copy the graphics on a window
    virtual void copyToWindow( OSWindow &rWindow, int xSrc,
                               int ySrc, int width, int height,
                               int xDest, int yDest ) = 0;

    /// Tell whether the pixel at the given position is visible
    virtual bool hit( int x, int y ) const = 0;

protected:
    OSGraphics( intf_thread_t *pIntf ): SkinObject( pIntf ) { }
};

typedef CountedPtr<OSGraphics> OSGraphicsPtr;

#endif
