/*****************************************************************************
 * V210.hpp: V210 picture conversion
 *****************************************************************************
 * Copyright © 2014-2016 VideoLAN and VideoLAN Authors
 *                  2018 VideoLabs
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef V210_HPP
#define V210_HPP

#include <vlc_common.h>

namespace sdi
{

    class V210
    {
        public:
            static const int ALIGNMENT_U16 = 6;
            static void Convert(const picture_t *, unsigned, void *);
            static void Convert(const uint16_t *, size_t, void *);
    };

}

#endif // V210_HPP
