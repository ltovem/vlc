// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * HLSSegment.hpp
 *****************************************************************************
 Copyright (C) 2015 VideoLAN and VLC authors
 *****************************************************************************/
#ifndef HLSSEGMENT_HPP
#define HLSSEGMENT_HPP

#include "../../adaptive/playlist/Segment.h"
#include "../../adaptive/encryption/CommonEncryption.hpp"

namespace hls
{
    namespace playlist
    {
        using namespace adaptive;
        using namespace adaptive::playlist;
        using namespace adaptive::encryption;

        class HLSSegment : public Segment
        {
            friend class M3U8Parser;

            public:
                HLSSegment( ICanonicalUrl *parent, uint64_t sequence );
                virtual ~HLSSegment();

            protected:
                virtual bool prepareChunk(SharedResources *, SegmentChunk *,
                                          BaseRepresentation *) override;
        };
    }
}


#endif // HLSSEGMENT_HPP
