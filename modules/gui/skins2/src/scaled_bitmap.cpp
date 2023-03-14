/*****************************************************************************
 * scaled_bitmap.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "scaled_bitmap.hpp"


ScaledBitmap::ScaledBitmap( intf_thread_t *pIntf, const GenericBitmap &rBitmap,
                            int width, int height ):
    GenericBitmap( pIntf ), m_width( width ), m_height( height )
{
    unsigned size;

    if (mul_overflow((unsigned)width, (unsigned)height, &size)
     || mul_overflow(size, 4, &size))
        throw std::bad_alloc();

    // Allocate memory for the buffer
    m_pData = new uint8_t[size];

    int srcWidth = rBitmap.getWidth();
    int srcHeight = rBitmap.getHeight();
    uint32_t *pSrcData = (uint32_t*)rBitmap.getData();
    uint32_t *pDestData = (uint32_t*)m_pData;

    // Algorithm for horizontal enlargement
    if( width > srcWidth )
    {
        // Decision variables for Bresenham algorithm
        int incX1 = 2 * (srcWidth-1);
        int incX2 = incX1 - 2 * (width-1);

        for( int y = 0; y < height; y++ )
        {
            int dX = incX1 - (width-1);
            uint32_t yOffset = ((y * srcHeight) / height) * srcWidth;
            pSrcData = ((uint32_t*)rBitmap.getData()) + yOffset;

            for( int x = 0; x < width; x++ )
            {
                *(pDestData++) = *pSrcData;

                if( dX <= 0 )
                {
                    dX += incX1;
                }
                else
                {
                    dX += incX2;
                    pSrcData++;
                }
            }
        }
    }
    // Algorithm for horizontal reduction
    else
    {
        // Decision variables for Bresenham algorithm
        int incX1 = 2 * (width-1);
        int incX2 = incX1 - 2 * (srcWidth-1);

        for( int y = 0; y < height; y++ )
        {
            int dX = incX1 - (srcWidth-1);
            uint32_t yOffset = ((y * srcHeight) / height) * srcWidth;
            pSrcData = ((uint32_t*)rBitmap.getData()) + yOffset;

            if (width == 1)
            {
                *(pDestData++) = *pSrcData;
            }
            else for( int x = 0; x < width; x++ )
            {
                *(pDestData++) = *(pSrcData++);

                while( dX <= 0 )
                {
                    dX += incX1;
                    pSrcData++;
                }
                dX += incX2;
            }
        }

    }
}


ScaledBitmap::~ScaledBitmap()
{
    delete[] m_pData;
}

