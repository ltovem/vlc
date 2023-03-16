/*****************************************************************************
 * file_bitmap.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef FILE_BITMAP_HPP
#define FILE_BITMAP_HPP

#include "generic_bitmap.hpp"
#include <string>


/// Class for file bitmaps
class FileBitmap: public GenericBitmap
{
public:
    /// Load a bitmap from a file. aColor is the transparency
    /// color, in the format 0xRRGGBB
    FileBitmap( intf_thread_t *pIntf, image_handler_t *pImageHandler,
                std::string fileName, uint32_t aColor, int nbFrames = 1,
                int fps = 0, int nbLoops = 0 );

    virtual ~FileBitmap();

    /// Get the width of the bitmap
    virtual int getWidth() const { return m_width; }

    /// Get the heighth of the bitmap
    virtual int getHeight() const { return m_height; }

    /// Get a linear buffer containing the image data.
    /// Each pixel is stored in 4 bytes in the order B,G,R,A
    virtual uint8_t *getData() const;

private:
    /// Size of the bitmap.
    int m_width, m_height;
    /// Buffer containing the image data.
    uint8_t *m_pData;
};


#endif
