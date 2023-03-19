// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Segment.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef SEGMENT_H_
#define SEGMENT_H_

#include <string>
#include <sstream>
#include <vector>
#include "ICanonicalUrl.hpp"
#include "../http/Chunk.h"
#include "../encryption/CommonEncryption.hpp"
#include "../tools/Properties.hpp"
#include "../Time.hpp"

namespace adaptive
{
    class SharedResources;

    namespace playlist
    {
        class BaseRepresentation;
        class SubSegment;
        class Segment;
        class SegmentChunk;

        using namespace http;
        using namespace encryption;

        class ISegment : public ICanonicalUrl
        {
            public:
                ISegment(const ICanonicalUrl *parent);
                virtual ~ISegment();
                /**
                 *  @return true if the segment should be dropped after being read.
                 *          That is basically true when using an Url, and false
                 *          when using an UrlTemplate
                 */
                virtual SegmentChunk*                   toChunk         (SharedResources *, size_t, BaseRepresentation *);
                virtual SegmentChunk*                   createChunk     (AbstractChunkSource *, BaseRepresentation *) = 0;
                virtual void                            setByteRange    (size_t start, size_t end);
                virtual void                            setSequenceNumber(uint64_t);
                virtual uint64_t                        getSequenceNumber() const;
                virtual void                            setDiscontinuitySequenceNumber(uint64_t);
                virtual uint64_t                        getDiscontinuitySequenceNumber() const;
                virtual bool                            isTemplate      () const;
                virtual size_t                          getOffset       () const;
                virtual void                            debug           (vlc_object_t *,int = 0) const;
                virtual bool                            contains        (size_t byte) const;
                void                                    setEncryption   (CommonEncryption &);
                void                                    setDisplayTime  (vlc_tick_t);
                vlc_tick_t                              getDisplayTime  () const;
                Property<stime_t>       startTime;
                Property<stime_t>       duration;
                bool                    discontinuity;

            protected:
                virtual bool                            prepareChunk    (SharedResources *,
                                                                         SegmentChunk *,
                                                                         BaseRepresentation *);
                CommonEncryption        encryption;
                size_t                  startByte;
                size_t                  endByte;
                std::string             debugName;
                bool                    templated;
                uint64_t                sequence;
                uint64_t                discontinuitySequenceNumber;
                vlc_tick_t              displayTime;
        };

        class Segment : public ISegment
        {
            public:
                Segment( ICanonicalUrl *parent );
                ~Segment();
                virtual SegmentChunk* createChunk(AbstractChunkSource *, BaseRepresentation *) override;
                virtual void setSourceUrl( const std::string &url );
                virtual Url getUrlSegment() const override;
                virtual const std::vector<Segment*> & subSegments() const;
                virtual void debug(vlc_object_t *,int = 0) const override;
                virtual void addSubSegment(SubSegment *);

            protected:
                std::vector<Segment *> subsegments;
                Url sourceUrl;
        };

        class InitSegment : public Segment
        {
            public:
                InitSegment( ICanonicalUrl *parent );
        };

        class IndexSegment : public Segment
        {
            public:
                IndexSegment( ICanonicalUrl *parent );
        };

        class SubSegment : public Segment
        {
            public:
                SubSegment(Segment *, size_t start, size_t end);
        };
    }
}

#endif /* SEGMENT_H_ */
