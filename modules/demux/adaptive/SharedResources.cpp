/*
 * SharedResources.cpp
 *****************************************************************************
 * Copyright © 2019 VideoLabs, VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "SharedResources.hpp"
#include "http/AuthStorage.hpp"
#include "http/HTTPConnectionManager.h"
#include "http/HTTPConnection.hpp"
#include "encryption/Keyring.hpp"

using namespace adaptive;

SharedResources::SharedResources(AuthStorage *auth, Keyring *ring,
                                 AbstractConnectionManager *conn)
{
    authStorage = auth;
    encryptionKeyring = ring;
    connManager = conn;
}

SharedResources::~SharedResources()
{
    delete connManager;
    delete encryptionKeyring;
    delete authStorage;
}

AuthStorage * SharedResources::getAuthStorage()
{
    return authStorage;
}

Keyring * SharedResources::getKeyring()
{
    return encryptionKeyring;
}

AbstractConnectionManager * SharedResources::getConnManager()
{
    return connManager;
}

SharedResources * SharedResources::createDefault(vlc_object_t *obj,
                                                 const std::string & playlisturl)
{
    AuthStorage *auth = new AuthStorage(obj);
    Keyring *keyring = new Keyring(obj);
    HTTPConnectionManager *m = new HTTPConnectionManager(obj);
    if(!var_InheritBool(obj, "adaptive-use-access")) /* only use http from access */
        m->addFactory(new LibVLCHTTPConnectionFactory(auth));
    m->addFactory(new StreamUrlConnectionFactory());
    ConnectionParams params(playlisturl);
    if(params.isLocal())
        m->setLocalConnectionsAllowed();
    return new SharedResources(auth, keyring, m);
}
