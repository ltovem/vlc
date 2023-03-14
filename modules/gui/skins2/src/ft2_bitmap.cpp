/*****************************************************************************
 * ft2_bitmap.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "ft2_bitmap.hpp"


FT2Bitmap::FT2Bitmap( intf_thread_t *pIntf, int width, int height ):
    GenericBitmap( pIntf ), m_width( width ), m_height( height )
{
    unsigned size;

    if (mul_overflow((unsigned)width, (unsigned)height, &size)
     || mul_overflow(size, 4, &size))
        throw std::bad_alloc();

    // Allocate memory for the buffer
    m_pData = new uint8_t[size];
    memset(m_pData, 0, size);
}


FT2Bitmap::~FT2Bitmap()
{
    delete[] m_pData;
}


void FT2Bitmap::draw( const FT_Bitmap &rBitmap, int left, int top,
                      uint32_t color )
{
    uint8_t *pBuf = rBitmap.buffer;

    // Calculate colors
    uint8_t blue = color & 0xff;
    uint8_t green = (color >> 8) & 0xff;
    uint8_t red = (color >> 16) & 0xff;

    for( unsigned y = top; y < top + rBitmap.rows && y < m_height; y++ )
    {
        uint8_t *pData = m_pData + 4 * (m_width * y + left);
        for( unsigned x = left; x < left + rBitmap.width && x < m_width; x++ )
        {
            // The buffer in FT_Bitmap contains alpha values
            uint8_t val = *(pBuf++);
            *(pData++) = (blue * val) >> 8;
            *(pData++) = (green * val) >> 8;
            *(pData++) = (red * val) >> 8;
            *(pData++) = val;
        }
    }
}


