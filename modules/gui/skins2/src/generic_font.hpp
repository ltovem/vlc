// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * generic_font.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef GENERIC_FONT_HPP
#define GENERIC_FONT_HPP

#include "skin_common.hpp"
#include "../utils/pointer.hpp"

class GenericBitmap;
class UString;

/// Base class for fonts
class GenericFont: public SkinObject
{
public:
    virtual ~GenericFont() { }

    virtual bool init() = 0;

    /// Render a string on a bitmap.
    /// If maxWidth != -1, the text is truncated with '...'
    /// The Bitmap is _not_ owned by this object
    virtual GenericBitmap *drawString( const UString &rString,
        uint32_t color, int maxWidth = -1 ) const = 0;

    /// Get the font size
    virtual int getSize() const = 0;

protected:
    GenericFont( intf_thread_t *pIntf ): SkinObject( pIntf ) { }
};

typedef CountedPtr<GenericFont> GenericFontPtr;


#endif
