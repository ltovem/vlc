/*
 * SharedResources.h
 *****************************************************************************
 * Copyright © 2019 VideoLabs, VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef SHAREDRESOURCES_H_
#define SHAREDRESOURCES_H_

#include <vlc_common.h>
#include <string>

namespace adaptive
{
    namespace http
    {
        class AuthStorage;
        class AbstractConnectionManager;
    }

    namespace encryption
    {
        class Keyring;
    }

    using namespace http;
    using namespace encryption;

    class SharedResources
    {
        public:
            SharedResources(AuthStorage *, Keyring *, AbstractConnectionManager *);
            ~SharedResources();
            AuthStorage *getAuthStorage();
            Keyring     *getKeyring();
            AbstractConnectionManager *getConnManager();
            /* Helper */
            static SharedResources * createDefault(vlc_object_t *, const std::string &);

        private:
            AuthStorage *authStorage;
            Keyring *encryptionKeyring;
            AbstractConnectionManager *connManager;
    };
}

#endif
