/*
 * SegmentList.h
 *****************************************************************************
 * Copyright (C) 2010 - 2012 Klagenfurt University
 *
 * Created on: Jan 27, 2012
 * Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
 *          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef SEGMENTLIST_H_
#define SEGMENTLIST_H_

#include "SegmentBaseType.hpp"
#include "Inheritables.hpp"

namespace adaptive
{
    namespace playlist
    {
        class SegmentInformation;
        class Segment;

        class SegmentList : public AbstractMultipleSegmentBaseType
        {
            public:
                SegmentList             ( SegmentInformation * = nullptr, bool = false );
                virtual ~SegmentList    ();

                const std::vector<Segment *>&   getSegments() const;
                void                    addSegment(Segment *seg);
                virtual void            updateWith(AbstractMultipleSegmentBaseType *,
                                                   bool = false) override;
                void                    pruneBySegmentNumber(uint64_t);
                void                    pruneByPlaybackTime(vlc_tick_t);
                stime_t                 getTotalLength() const;
                bool                    hasRelativeMediaTimes() const;

                virtual vlc_tick_t  getMinAheadTime(uint64_t) const override;
                virtual Segment * getMediaSegment(uint64_t pos) const override;
                virtual Segment * getNextMediaSegment(uint64_t, uint64_t *, bool *) const override;
                virtual uint64_t  getStartSegmentNumber() const override;
                virtual bool getSegmentNumberByTime(vlc_tick_t time, uint64_t *ret) const override;
                virtual bool getPlaybackTimeDurationBySegmentNumber(uint64_t number,
                                            vlc_tick_t *time, vlc_tick_t *duration) const override;

                virtual void debug(vlc_object_t *, int = 0) const override;

            private:
                std::vector<Segment *>  segments;
                stime_t totalLength;
                bool b_relative_mediatimes;
        };
    }
}

#endif /* SEGMENTLIST_H_ */
