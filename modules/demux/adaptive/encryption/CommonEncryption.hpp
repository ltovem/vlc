// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * CommonEncryption.hpp
 *****************************************************************************
 * Copyright (C) 2015-2019 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef COMMONENCRYPTION_H
#define COMMONENCRYPTION_H

#include <vector>
#include <string>

namespace adaptive
{
    class SharedResources;

    namespace encryption
    {
        class CommonEncryption
        {
            public:
                CommonEncryption();
                void mergeWith(const CommonEncryption &);
                enum class Method
                {
                    None,
                    AES_128,
                    AES_Sample,
                } method;
                std::string uri;
                std::vector<unsigned char> iv;
        };

        class CommonEncryptionSession
        {
            public:
                CommonEncryptionSession();
                ~CommonEncryptionSession();

                bool start(SharedResources *, const CommonEncryption &);
                void close();
                size_t decrypt(void *, size_t, bool);

            private:
                std::vector<unsigned char> key;
                CommonEncryption encryption;
                void *ctx;
        };
    }
}

#endif
