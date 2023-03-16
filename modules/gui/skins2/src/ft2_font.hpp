// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * ft2_font.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef FT2_FONT_HPP
#define FT2_FONT_HPP

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <string>
#include <map>

#include "generic_font.hpp"

class UString;


/// Freetype2 font
class FT2Font: public GenericFont
{
public:
    FT2Font( intf_thread_t *pIntf, const std::string &rName, int size );
    virtual ~FT2Font();

    /// Initialize the object. Returns false if it failed
    virtual bool init();

    /// Render a string on a bitmap.
    /// If maxWidth != -1, the text is truncated with '...'
    virtual GenericBitmap *drawString( const UString &rString,
        uint32_t color, int maxWidth = -1 ) const;

    /// Get the text height
    virtual int getSize() const { return m_height; }

private:
    typedef struct
    {
        FT_Glyph m_glyph;
        FT_BBox m_size;
        int m_index;
        int m_advance;
    } Glyph_t;
    typedef std::map<uint32_t,Glyph_t> GlyphMap_t;

    /// File name
    const std::string m_name;
    /// Buffer to store the font
    char *m_buffer;
    /// Pixel size of the font
    int m_size;
    /// Handle to FT library
    FT_Library m_lib;
    /// Font face
    FT_Face m_face;
    /// Font metrics
    int m_height, m_ascender, m_descender;
    /// Glyph cache
    mutable GlyphMap_t m_glyphCache;

    /// Get the glyph corresponding to the given code
    Glyph_t &getGlyph( uint32_t code ) const;
    bool error( unsigned err, const char *msg );
};


#endif
