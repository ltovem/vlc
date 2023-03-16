// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * ConnectionParams.hpp
 *****************************************************************************
 Copyright (C) 2016 - VideoLAN and VLC Authors
 *****************************************************************************/
#ifndef CONNECTIONPARAMS_HPP
#define CONNECTIONPARAMS_HPP

#include <vlc_common.h>
#include <string>

namespace adaptive
{
    namespace http
    {
        class Transport;

        enum class RequestStatus
        {
            Success,
            Redirection,
            Unauthorized,
            NotFound,
            GenericError,
        };

        class BackendPrefInterface
        {
            /* Design Hack for now to force fallback on regular access
             * through hybrid connection factory */
            public:
                BackendPrefInterface() { useaccess = false; }
                bool usesAccess() const { return useaccess; }
                void setUseAccess(bool b) { useaccess = b; }
            private:
                bool useaccess;
        };

        class ConnectionParams : public BackendPrefInterface
        {
            public:
                ConnectionParams();
                ConnectionParams(const std::string &);
                const std::string & getUrl() const;
                const std::string & getScheme() const;
                const std::string & getHostname() const;
                const std::string & getPath() const;
                bool isLocal() const;
                void setPath(const std::string &);
                uint16_t getPort() const;

            private:
                void parse();
                std::string uri;
                std::string scheme;
                std::string hostname;
                std::string path;
                uint16_t port;
        };
    }
}

#endif // CONNECTIONPARAMS_HPP
