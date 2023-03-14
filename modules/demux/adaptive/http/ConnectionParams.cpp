/*
 * ConnectionParams.cpp
 *****************************************************************************
 * Copyright (C) 2016 - VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "ConnectionParams.hpp"

#include <vlc_url.h>
#include <ctype.h>
#include <algorithm>

using namespace adaptive::http;

ConnectionParams::ConnectionParams()
{
}

ConnectionParams::ConnectionParams(const std::string &uri)
{
    this->uri = uri;
    parse();
}

const std::string & ConnectionParams::getUrl() const
{
    return uri;
}

const std::string & ConnectionParams::getScheme() const
{
    return scheme;
}

const std::string & ConnectionParams::getHostname() const
{
    return hostname;
}

const std::string & ConnectionParams::getPath() const
{
    return path;
}

void ConnectionParams::setPath(const std::string &path_)
{
    vlc_url_t url_components{};
    char *uri_str;

    url_components.psz_protocol = const_cast<char *>(scheme.c_str());
    url_components.psz_path = const_cast<char *>(path_.c_str());
    url_components.i_port = 0;

    if (!hostname.empty()) {
        url_components.psz_host = const_cast<char *>(hostname.c_str());
        if ((port != 80 && scheme == "http") ||
            (port != 443 && scheme == "https"))
            url_components.i_port = port;
    }

    uri_str = vlc_uri_compose(&url_components);
    if (uri_str != nullptr) {
        path = path_;
        uri.assign(uri_str);
    }
    free(uri_str);
}

uint16_t ConnectionParams::getPort() const
{
    return port;
}

bool ConnectionParams::isLocal() const
{
    return scheme != "http" && scheme != "https";
}

void ConnectionParams::parse()
{
    vlc_url_t url_components;
    vlc_UrlParse(&url_components, uri.c_str());

    if(url_components.psz_protocol)
    {
        scheme = url_components.psz_protocol;
        std::transform(scheme.begin(), scheme.end(), scheme.begin(), tolower);
    }
    if(url_components.psz_path)
        path = url_components.psz_path;
    if(url_components.psz_option)
    {
        path += "?";
        path += url_components.psz_option;
    }
    port = url_components.i_port ? url_components.i_port :
                         ((scheme == "https") ? 443 : 80);
    if(url_components.psz_host)
        hostname = url_components.psz_host;

    vlc_UrlClean(&url_components);
}
