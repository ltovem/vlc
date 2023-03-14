/*****************************************************************************
 * SmoothSegment.hpp:
 *****************************************************************************
 * Copyright (C) 2015 - VideoLAN Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef SMOOTHSEGMENT_HPP
#define SMOOTHSEGMENT_HPP

#include "../../adaptive/playlist/SegmentTemplate.h"
#include "../../adaptive/playlist/SegmentChunk.hpp"

namespace smooth
{
    namespace playlist
    {
        using namespace adaptive::playlist;

        class SmoothSegmentChunk : public SegmentChunk
        {
            public:
                SmoothSegmentChunk(AbstractChunkSource *, BaseRepresentation *);
                virtual ~SmoothSegmentChunk();
                virtual void onDownload(block_t **) override;
        };

        class SmoothSegmentTemplateSegment : public SegmentTemplateSegment
        {
            public:
                SmoothSegmentTemplateSegment( ICanonicalUrl * = nullptr );
                virtual ~SmoothSegmentTemplateSegment();
                virtual SegmentChunk* createChunk(AbstractChunkSource *, BaseRepresentation *) override;
        };
    }
}
#endif // SMOOTHSEGMENT_HPP
