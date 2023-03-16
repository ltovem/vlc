// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * AbstractSource.hpp
 *****************************************************************************
 Copyright © 2015-2019 - VideoLabs, VideoLAN and VLC Authors
 *****************************************************************************/
#ifndef ABSTRACTSOURCE_HPP
#define ABSTRACTSOURCE_HPP

#include <vlc_common.h>

namespace adaptive
{
    class AbstractSource
    {
        public:
            virtual ~AbstractSource() {}
            virtual block_t *readNextBlock() = 0;
    };
}

#endif // ABSTRACTSOURCE_HPP

