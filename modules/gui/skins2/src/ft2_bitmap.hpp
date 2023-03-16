/*****************************************************************************
 * ft2_bitmap.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef FT2_BITMAP_HPP
#define FT2_BITMAP_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

#include "generic_bitmap.hpp"


/// Class for rendering freetype glyphs on a bitmap
class FT2Bitmap: public GenericBitmap
{
public:
    /// Create an empty bitmap
    FT2Bitmap( intf_thread_t *pIntf, int width, int height );

    virtual ~FT2Bitmap();

    /// Get the width of the bitmap
    virtual int getWidth() const { return m_width; }

    /// Get the heighth of the bitmap
    virtual int getHeight() const { return m_height; }

    /// Get a linear buffer containing the image data.
    /// Each pixel is stored in 4 bytes in the order B,G,R,A
    virtual uint8_t *getData() const { return m_pData; }

    /// Render a glyph on the bitmap at the given position
    void draw( const FT_Bitmap &rBitmap, int left, int top,
               uint32_t color );

private:
    /// Bitmap size
    unsigned int m_width, m_height;
    /// Image data buffer
    uint8_t *m_pData;
};


#endif
