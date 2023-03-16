// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * SmoothIndexReader.hpp
 *****************************************************************************
 Copyright (C) 2015 - VideoLAN and VLC authors
 *****************************************************************************/
#ifndef SMOOTHINDEXREADER_HPP
#define SMOOTHINDEXREADER_HPP

#include "../../adaptive/mp4/AtomsReader.hpp"

namespace adaptive
{
    namespace playlist
    {
        class BaseRepresentation;
    }
}

namespace smooth
{
    namespace mp4
    {
        using namespace adaptive::mp4;
        using namespace adaptive::playlist;

        class SmoothIndexReader : public AtomsReader
        {
            public:
                SmoothIndexReader(vlc_object_t *);
                bool parseIndex(block_t *, BaseRepresentation *, uint64_t);
        };
    }
}

#endif // SMOOTHINDEXREADER_HPP
