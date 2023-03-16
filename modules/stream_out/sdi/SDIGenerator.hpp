// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * SDIGenerator.hpp: SDI Image and sound generators
 *****************************************************************************
 * Copyright © 2014-2016 VideoLAN and VideoLAN Authors
 *             2018-2019 VideoLabs
 *****************************************************************************/
#ifndef SDIGENERATOR_HPP
#define SDIGENERATOR_HPP

namespace sdi
{
    class Generator
    {
        public:
            static picture_t * Picture(vlc_object_t *, const char*,
                                       const video_format_t *);
    };
}

#endif
