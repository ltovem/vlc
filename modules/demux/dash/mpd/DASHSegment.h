// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Segment.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef DASHSEGMENT_H_
#define DASHSEGMENT_H_

#include "../../adaptive/playlist/Segment.h"
#include "../../adaptive/playlist/SegmentChunk.hpp"

namespace dash
{
    namespace mpd
    {
        using namespace adaptive::playlist;
        using namespace adaptive::http;

        class DashIndexChunk : public SegmentChunk
        {
            public:
                DashIndexChunk(AbstractChunkSource *, BaseRepresentation *);
                ~DashIndexChunk();
                virtual void onDownload(block_t **) override;
        };

        class DashIndexSegment : public IndexSegment
        {
            public:
                DashIndexSegment( ICanonicalUrl *parent );
                ~DashIndexSegment();
                virtual SegmentChunk* createChunk(AbstractChunkSource *, BaseRepresentation *) override;
        };

    }
}

#endif /* DASHSEGMENT_H_ */
