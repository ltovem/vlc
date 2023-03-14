/*
 * AuthStorage.hpp
 *****************************************************************************
 * Copyright (C) 2017 - VideoLabs and VideoLAN Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef AUTHSTORAGE_HPP_
#define AUTHSTORAGE_HPP_

#include <vlc_common.h>
#include <vlc_http.h>

#include <string>

namespace adaptive
{
    namespace http
    {
        class ConnectionParams;

        class AuthStorage
        {
            public:
                AuthStorage(vlc_object_t *p_obj);
                ~AuthStorage();
                void addCookie( const std::string &cookie, const ConnectionParams & );
                std::string getCookie( const ConnectionParams &, bool secure );
                vlc_http_cookie_jar_t *getJar() const;

            private:
                vlc_http_cookie_jar_t *p_cookies_jar;
        };
    }
}

#endif
