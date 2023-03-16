/*****************************************************************************
 * bitmap_font.hpp
 *****************************************************************************
 * Copyright (C) 2004 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef BITMAP_FONT_HPP
#define BITMAP_FONT_HPP

#include "generic_font.hpp"
#include <string>

class GenericBitmap;


/// Class to handle bitmap fonts
class BitmapFont: public GenericFont
{
public:
    BitmapFont( intf_thread_t *pIntf, const GenericBitmap &rBitmap,
                const std::string &rType );
    virtual ~BitmapFont() { }

    virtual bool init() { return true; }

    /// Render a string on a bitmap.
    /// If maxWidth != -1, the text is truncated with '...'
    virtual GenericBitmap *drawString( const UString &rString,
        uint32_t color, int maxWidth = -1 ) const;

    /// Get the font size
    virtual int getSize() const { return m_height; }

private:
    /// Description of a glyph
    struct Glyph_t
    {
        Glyph_t(): m_xPos( -1 ), m_yPos( 0 ) { }
        int m_xPos, m_yPos;
    };

    /// Bitmap
    const GenericBitmap &m_rBitmap;
    /// Glyph size
    int m_width, m_height;
    /// Horizontal advance between two characters
    int m_advance;
    /// Horizontal advance for non-displayable characters
    int m_skip;
    /// Character table
    Glyph_t m_table[256];
};

#endif
