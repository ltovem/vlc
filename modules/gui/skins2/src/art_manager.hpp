/*****************************************************************************
 * art_manager.hpp
 *****************************************************************************
 * Copyright (C) 2010 the VideoLAN team
 *
 * Author: Erwan Tulou      <erwan10@vidoelan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef ART_MANAGER_HPP
#define ART_MANAGER_HPP

#include "file_bitmap.hpp"
#include <string>
#include <list>


/// Class for art bitmaps
class ArtBitmap: public FileBitmap
{
public:

    std::string getUriName() { return m_uriName; }

    /// Constructor/destructor
    ArtBitmap( intf_thread_t *pIntf, image_handler_t *pImageHandler,
               std::string uriName ) :
        FileBitmap( pIntf, pImageHandler, uriName, -1 ),
        m_uriName( uriName ) {}
    virtual ~ArtBitmap() {}

private:
    /// uriName
    std::string m_uriName;
};


/// Singleton object for handling art
class ArtManager: public SkinObject
{
public:
    /// Get the instance of ArtManager
    /// Returns NULL if the initialization of the object failed
    static ArtManager *instance( intf_thread_t *pIntf );

    /// Delete the instance of ArtManager
    static void destroy( intf_thread_t *pIntf );

    /// Retrieve for the art file from uri name
    ArtBitmap* getArtBitmap( std::string uriName );

protected:
    // Protected because it is a singleton
    ArtManager( intf_thread_t *pIntf );
    virtual ~ArtManager();

private:
    /// Image handler (used to load art files)
    image_handler_t *m_pImageHandler;

    // keep a cache of art already open
    std::list<ArtBitmap*> m_listBitmap;
};

#endif
