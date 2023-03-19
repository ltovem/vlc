// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * generic_bitmap.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef GENERIC_BITMAP_HPP
#define GENERIC_BITMAP_HPP

#include "skin_common.hpp"
#include "../src/os_graphics.hpp"
#include "../utils/pointer.hpp"
#include "../utils/position.hpp"


/// Generic interface for bitmaps
class GenericBitmap: public SkinObject, public Box
{
public:
    virtual ~GenericBitmap() { delete m_pGraphics; }

    /// Get a linear buffer containing the image data.
    /// Each pixel is stored in 4 bytes in the order B,G,R,A
    virtual uint8_t *getData() const = 0;

    /// Get the bitmap as a graphics
    virtual const OSGraphics *getGraphics() const;

    /// Get the number of frames in the bitmap
    int getNbFrames() const { return m_nbFrames; }

    /// Get the number of frames per second (for animated bitmaps)
    int getFrameRate() const { return m_frameRate; }

    /// Get the number of Loops (for animated bitmaps)
    int getNbLoops() const { return m_nbLoops; }

protected:
    GenericBitmap( intf_thread_t *pIntf, int nbFrames = 1, int fps = 0, int nbLoops = 0);

private:
    /// Number of frames
    int m_nbFrames;
    /// Frame rate
    int m_frameRate;
    /// Number of Loops
    int m_nbLoops;

    /// graphics copy of the bitmap
    mutable OSGraphics* m_pGraphics;
};


/// Basic bitmap implementation
class BitmapImpl: public GenericBitmap
{
public:
    /// Create an empty bitmap of the given size
    BitmapImpl( intf_thread_t *pIntf, int width, int height,
                int nbFrames = 1, int fps = 0, int nbLoops = 0 );
    ~BitmapImpl();

    /// Get the width of the bitmap
    virtual int getWidth() const { return m_width; }

    /// Get the heighth of the bitmap
    virtual int getHeight() const { return m_height; }

    /// Get a linear buffer containing the image data.
    /// Each pixel is stored in 4 bytes in the order B,G,R,A
    virtual uint8_t *getData() const { return m_pData; }

    // Copy a region of another bitmap on this bitmap
    bool drawBitmap( const GenericBitmap &rSource, int xSrc, int ySrc,
                     int xDest, int yDest, int width, int height );

private:
    /// Size of the bitmap.
    int m_width, m_height;
    /// Buffer containing the image data.
    uint8_t *m_pData;
};


typedef CountedPtr<GenericBitmap> GenericBitmapPtr;

#endif
