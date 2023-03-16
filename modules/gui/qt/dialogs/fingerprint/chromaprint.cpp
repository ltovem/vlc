// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * chromaprint.cpp: Fingerprinter helper class
 *****************************************************************************
 * Copyright (C) 2012 VLC authors and VideoLAN
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "chromaprint.hpp"
#include <vlc_modules.h>

Chromaprint::Chromaprint( qt_intf_t *_p_intf ) : p_intf( _p_intf )
{
    Q_ASSERT( p_intf );
    p_fingerprinter = fingerprinter_Create( VLC_OBJECT( p_intf ) );
    if ( p_fingerprinter )
        var_AddCallback( p_fingerprinter, "results-available", results_available, this );
}

int Chromaprint::results_available( vlc_object_t *, const char *, vlc_value_t, vlc_value_t , void *param )
{
    Chromaprint *me = (Chromaprint *) param;
    me->finish();
    return 0;
}

fingerprint_request_t * Chromaprint::fetchResults()
{
    return p_fingerprinter->pf_getresults( p_fingerprinter );
}

void Chromaprint::apply( fingerprint_request_t *p_r, size_t i_id )
{
    p_fingerprinter->pf_apply( p_r, i_id );
}

bool Chromaprint::enqueue( input_item_t *p_item )
{
    if ( ! p_fingerprinter ) return false;
    fingerprint_request_t *p_r = fingerprint_request_New( p_item );
    if ( ! p_r ) return false;
    vlc_tick_t t = input_item_GetDuration( p_item );
    if ( t != 0 ) p_r->i_duration = (unsigned int) SEC_FROM_VLC_TICK( t );
    if( p_fingerprinter->pf_enqueue( p_fingerprinter, p_r ) != 0 )
    {
        fingerprint_request_Delete( p_r );
        return false;
    }
    return true;
}

bool Chromaprint::isSupported( QString uri )
{
    if ( !module_exists( "stream_out_chromaprint" ) )
        return false;
    else
    return ( uri.startsWith( "file://" ) || uri.startsWith( "/" ) );
}

Chromaprint::~Chromaprint()
{
    if ( p_fingerprinter )
        fingerprinter_Destroy( p_fingerprinter );
}
