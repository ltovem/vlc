/*
 * IndexReader.hpp
 *****************************************************************************
 * Copyright (C) 2015 - VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef INDEXREADER_HPP
#define INDEXREADER_HPP

#include "../../adaptive/mp4/AtomsReader.hpp"

namespace adaptive
{
    namespace playlist
    {
        class BaseRepresentation;
    }
}

namespace dash
{
    namespace mp4
    {
        using namespace adaptive::mp4;
        using namespace adaptive::playlist;

        class IndexReader : public AtomsReader
        {
            public:
                IndexReader(vlc_object_t *);
                bool parseIndex(block_t *, BaseRepresentation *, uint64_t);
        };
    }
}

#endif // INDEXREADER_HPP
