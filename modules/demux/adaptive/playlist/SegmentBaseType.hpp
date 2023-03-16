// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * SegmentBaseType.hpp
 *****************************************************************************
 Copyright (C) 2020 VideoLabs, VideoLAN and VLC Authors
 *****************************************************************************/
#ifndef SEGMENTBASETYPE_H_
#define SEGMENTBASETYPE_H_

#include "Segment.h"
#include "Inheritables.hpp"
#include "Templates.hpp"

namespace adaptive
{
    namespace playlist
    {
        class SegmentInformation;

        class AbstractSegmentBaseType : public Initializable<InitSegment>,
                                        public Indexable<IndexSegment>,
                                        public AttrsNode
        {
            public:
                AbstractSegmentBaseType( SegmentInformation *, AttrsNode::Type );
                virtual ~AbstractSegmentBaseType();

                virtual vlc_tick_t getMinAheadTime(uint64_t) const = 0;
                virtual Segment *getMediaSegment(uint64_t pos) const = 0;
                virtual InitSegment *getInitSegment() const;
                virtual IndexSegment *getIndexSegment() const;
                virtual Segment *getNextMediaSegment(uint64_t, uint64_t *, bool *) const = 0;
                virtual uint64_t getStartSegmentNumber() const = 0;

                virtual bool getSegmentNumberByTime(vlc_tick_t time, uint64_t *ret) const = 0;
                virtual bool getPlaybackTimeDurationBySegmentNumber(uint64_t number,
                                                vlc_tick_t *time, vlc_tick_t *duration) const = 0;

                virtual void debug(vlc_object_t *, int = 0) const;

                static Segment * findSegmentByScaledTime(const std::vector<Segment *> &,
                                                         stime_t);
                static uint64_t findSegmentNumberByScaledTime(const std::vector<Segment *> &,
                                                             stime_t);

            protected:
                SegmentInformation *parent;
        };

        class AbstractMultipleSegmentBaseType : public AbstractSegmentBaseType
        {
            public:
                AbstractMultipleSegmentBaseType( SegmentInformation *, AttrsNode::Type );
                virtual ~AbstractMultipleSegmentBaseType();

                virtual void updateWith(AbstractMultipleSegmentBaseType *, bool = false);
        };
    }
}

#endif /* SEGMENTBASETYPE_H_ */
