/*****************************************************************************
 * anim_bitmap.hpp
 *****************************************************************************
 * Copyright (C) 2005 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef ANIM_BITMAP_HPP
#define ANIM_BITMAP_HPP

#include "../commands/cmd_generic.hpp"
#include "../utils/observer.hpp"
#include "../utils/position.hpp"

class GenericBitmap;
class OSGraphics;
class OSTimer;


/// Animated bitmap
class AnimBitmap: public SkinObject, public Box,
                  public Subject<AnimBitmap>
{
public:
    AnimBitmap( intf_thread_t *pIntf, const GenericBitmap &rBitmap );

    virtual ~AnimBitmap();

    /// Start the animation
    void startAnim();

    /// Stop the animation
    void stopAnim();

    /// Draw the current frame on another graphics
    void draw( OSGraphics &rImage, int xDest, int yDest, int w, int h, int xOffset = 0, int yOffset = 0 );

    /// Tell whether the pixel at the given position is visible
    bool hit( int x, int y ) const;

    /// Get the size of the current frame
    virtual int getWidth() const;
    virtual int getHeight() const;

    /// compare two animated image
    bool operator==( const AnimBitmap& other ) const;

private:
    /// Bitmap stored
    const GenericBitmap &m_rBitmap;
    /// Graphics to store the bitmap
    const OSGraphics * const m_pImage;
    /// Number of frames
    int m_nbFrames;
    /// Frame rate
    int m_frameRate;
    /// Number of Loops
    int m_nbLoops;
    /// Current frame
    int m_curFrame;
    /// Current loop
    int m_curLoop;
     /// Timer for the animation
    OSTimer *m_pTimer;

    /// Callback for the timer
    DEFINE_CALLBACK( AnimBitmap, NextFrame );
};


#endif

