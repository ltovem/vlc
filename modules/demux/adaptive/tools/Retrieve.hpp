/*
 * Retrieve.hpp
 *****************************************************************************
 * Copyright © 2015 - VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef RETRIEVE_HPP
#define RETRIEVE_HPP

#include <vlc_common.h>
#include <string>

namespace adaptive
{
    class SharedResources;
    namespace http
    {
        enum class ChunkType;
    };

    class Retrieve
    {
        public:
            static block_t * HTTP(SharedResources *, http::ChunkType, const std::string &uri);
    };
}

#endif // RETRIEVE_HPP
