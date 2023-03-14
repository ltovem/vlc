/*
 * AuthStorage.cpp
 *****************************************************************************
 * Copyright (C) 2017 - VideoLabs and VideoLAN Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "AuthStorage.hpp"
#include "ConnectionParams.hpp"

using namespace adaptive::http;

AuthStorage::AuthStorage( vlc_object_t *p_obj )
{
    if ( var_InheritBool( p_obj, "http-forward-cookies" ) )
        p_cookies_jar = static_cast<vlc_http_cookie_jar_t *>
                (var_InheritAddress( p_obj, "http-cookies" ));
    else
        p_cookies_jar = nullptr;
}

AuthStorage::~AuthStorage()
{
}

void AuthStorage::addCookie( const std::string &cookie, const ConnectionParams &params )
{
    if( !p_cookies_jar )
        return;
    vlc_http_cookies_store( p_cookies_jar, cookie.c_str(),
                            params.getHostname().c_str(), params.getPath().c_str() );
}

std::string AuthStorage::getCookie( const ConnectionParams &params, bool secure )
{
    if( !p_cookies_jar )
        return std::string();
    char *psz = vlc_http_cookies_fetch( p_cookies_jar, secure,
                                        params.getHostname().c_str(),
                                        params.getPath().c_str() );
    std::string ret;
    if( psz )
    {
        ret = std::string(psz);
        free( psz );
    }
    return ret;
}

vlc_http_cookie_jar_t *AuthStorage::getJar() const
{
    return p_cookies_jar;
}
