/*****************************************************************************
 * scaled_bitmap.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef SCALED_BITMAP_HPP
#define SCALED_BITMAP_HPP

#include "generic_bitmap.hpp"


/// Class for scaling bitmaps
class ScaledBitmap: public GenericBitmap
{
public:
    /// Create a scaled bitmap from the given bitmap and size
    ScaledBitmap( intf_thread_t *pIntf, const GenericBitmap &rBitmap,
                  int width, int height );

    virtual ~ScaledBitmap();

    /// Get the width of the bitmap
    virtual int getWidth() const { return m_width; }

    /// Get the heighth of the bitmap
    virtual int getHeight() const { return m_height; }

    /// Get a linear buffer containing the image data.
    /// Each pixel is stored in 4 bytes in the order B,G,R,A
    virtual uint8_t *getData() const { return m_pData; }

private:
    /// Bitmap size
    int m_width, m_height;
    /// Image data buffer
    uint8_t *m_pData;
};


#endif
