// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * AtomsReader.hpp
 *****************************************************************************
 Copyright (C) 2014 - VideoLAN and VLC authors
 *****************************************************************************/
#ifndef ATOMSREADER_HPP
#define ATOMSREADER_HPP

#include <vlc_common.h>
#include <vlc_stream.h>
#include "../../mp4/libmp4.h"

namespace adaptive
{
    namespace mp4
    {
        class AtomsReader
        {
            public:
                AtomsReader(vlc_object_t *);
                ~AtomsReader();
                void clean();
                bool parseBlock(block_t *);

            protected:
                vlc_object_t *object;
                MP4_Box_t *rootbox;
        };
    }
}

#endif // ATOMSREADER_HPP
