// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * art_manager.cpp
 *****************************************************************************
 * Copyright (C) 2010 the VideoLAN team
 *
 * Author: Erwan Tulou      <erwan10@vidoelan.org>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "art_manager.hpp"
#include <vlc_image.h>

#include <new>

#define MAX_ART_CACHED    2


ArtManager *ArtManager::instance( intf_thread_t *pIntf )
{
    if( pIntf->p_sys->p_artManager == NULL )
    {
        pIntf->p_sys->p_artManager = new ArtManager( pIntf );
    }

    return pIntf->p_sys->p_artManager;
}


void ArtManager::destroy( intf_thread_t *pIntf )
{
    delete pIntf->p_sys->p_artManager;
    pIntf->p_sys->p_artManager = NULL;
}


ArtManager::ArtManager( intf_thread_t* pIntf ) : SkinObject( pIntf )
{
    // initialize handler
    m_pImageHandler = image_HandlerCreate( pIntf );

    if( !m_pImageHandler )
        msg_Err( getIntf(), "initialization of art manager failed" );
}


ArtManager::~ArtManager( )
{
    if( m_pImageHandler )
    {
        image_HandlerDelete( m_pImageHandler );
        m_pImageHandler = NULL;
    }

    std::list<ArtBitmap*>::const_iterator it;
    for( it = m_listBitmap.begin(); it != m_listBitmap.end(); ++it )
        delete *it;
    m_listBitmap.clear();
}


ArtBitmap* ArtManager::getArtBitmap( std::string uriName )
{
    if( !uriName.size() )
        return NULL;

    if( !m_pImageHandler )
        return NULL;

    // check whether art is already loaded
    std::list<ArtBitmap*>::const_iterator it;
    for( it = m_listBitmap.begin(); it != m_listBitmap.end(); ++it )
    {
        if( (*it)->getUriName() == uriName )
            return *it;
    }

    // create and retain a new ArtBitmap since uri is not yet known
    ArtBitmap* pArt = new (std::nothrow) ArtBitmap( getIntf(), m_pImageHandler, uriName );
    if( pArt && pArt->getWidth() && pArt->getHeight() )
    {
        if( m_listBitmap.size() == MAX_ART_CACHED )
        {
            ArtBitmap* pOldest = *(m_listBitmap.begin());
            delete pOldest;
            m_listBitmap.pop_front();
        }
        m_listBitmap.push_back( pArt );
        return pArt;
    }
    else
    {
        delete pArt;
        return NULL;
    }
}
