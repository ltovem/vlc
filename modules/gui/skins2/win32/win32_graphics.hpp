/*****************************************************************************
 * win32_graphics.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef WIN32_GRAPHICS_HPP
#define WIN32_GRAPHICS_HPP

#include "../src/os_graphics.hpp"
#include <windows.h>


class GenericBitmap;

/// Win32 implementation of OSGraphics
class Win32Graphics: public OSGraphics
{
public:
    Win32Graphics( intf_thread_t *pIntf, int width, int height );
    virtual ~Win32Graphics();

    /// Clear the graphics
    virtual void clear( int xDest = 0, int yDest = 0,
                        int width = -1, int height = -1 );

    /// Render a bitmap on this graphics
    virtual void drawBitmap( const GenericBitmap &rBitmap, int xSrc = 0,
                             int ySrc = 0, int xDest = 0, int yDest = 0,
                             int width = -1, int height = -1,
                             bool blend = false );

    /// Draw another graphics on this one
    virtual void drawGraphics( const OSGraphics &rGraphics, int xSrc = 0,
                               int ySrc = 0, int xDest = 0, int yDest = 0,
                               int width = -1, int height = -1 );

    /// Draw an empty rectangle on the graphics (color is #RRGGBB)
    virtual void drawRect( int left, int top, int width, int height,
                           uint32_t color );


    /// Draw a filled rectangle on the graphics (color is #RRGGBB)
    virtual void fillRect( int left, int top, int width, int height,
                           uint32_t color );

    /// Set the shape of a window with the mask of this graphics.
    virtual void applyMaskToWindow( OSWindow &rWindow );

    /// Copy the graphics on a window
    virtual void copyToWindow( OSWindow &rWindow, int xSrc,
                               int ySrc, int width, int height,
                               int xDest, int yDest );

    /// Tell whether the pixel at the given position is visible
    virtual bool hit( int x, int y ) const;

    /// Getters for the size
    virtual int getWidth() const { return m_width; }
    virtual int getHeight() const { return m_height; }

    /// Get the device context handler
    virtual HDC getDC() const { return m_hDC; }

    /// Get the mask
    virtual HRGN getMask() const { return m_mask; }

private:
    /// Size of the image
    int m_width, m_height;

    /// Device context
    HDC m_hDC;

    /// Transparency mask
    HRGN m_mask;

    /// Add a segment in a region
    void addSegmentInRegion( HRGN &rMask, int start, int end, int line );

    /// check boundaries for graphics and bitmaps
    bool checkBoundaries( int x_src, int y_src, int w_src, int h_src,
                          int& x_target, int& y_target,
                          int& w_target, int& h_target );
};


#endif
