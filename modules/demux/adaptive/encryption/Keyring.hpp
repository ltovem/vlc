/*****************************************************************************
 * Keyring.hpp
 *****************************************************************************
 * Copyright (C) 2019 VideoLabs, VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef KEYRING_H
#define KEYRING_H

#include <vlc_common.h>

#include <map>
#include <list>
#include <vector>
#include <string>

namespace adaptive
{
    class SharedResources;

    namespace encryption
    {
        using KeyringKey = std::vector<unsigned char>;

        class Keyring
        {
            public:
                Keyring(vlc_object_t *);
                ~Keyring();
                KeyringKey getKey(SharedResources *, const std::string &);

            private:
                static const int MAX_KEYS = 50;
                std::map<std::string, KeyringKey> keys;
                std::list<std::string> lru;
                vlc_object_t *obj;
                vlc_mutex_t lock;
        };
    }
}

#endif
