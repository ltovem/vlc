/*
 * SegmentBase.h
 *****************************************************************************
 * Copyright (C) 2010 - 2012 Klagenfurt University
 *
 * Created on: Jan 27, 2012
 * Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
 *          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef SEGMENTBASE_H_
#define SEGMENTBASE_H_

#include "Segment.h"
#include "SegmentBaseType.hpp"

namespace adaptive
{
    namespace playlist
    {
        class SegmentInformation;

        /* SegmentBase can contain only one segment */
        class SegmentBase : public Segment,
                            public AbstractSegmentBaseType
        {
            public:
                SegmentBase             (SegmentInformation * = nullptr);
                virtual ~SegmentBase    ();

                virtual vlc_tick_t getMinAheadTime(uint64_t curnum) const override;
                virtual Segment *getMediaSegment(uint64_t number) const override;
                virtual Segment *getNextMediaSegment(uint64_t, uint64_t *, bool *) const override;
                virtual uint64_t getStartSegmentNumber() const override;

                virtual bool getSegmentNumberByTime(vlc_tick_t time, uint64_t *ret) const override;
                virtual bool getPlaybackTimeDurationBySegmentNumber(uint64_t number,
                                            vlc_tick_t *time, vlc_tick_t *duration) const override;

                virtual void debug(vlc_object_t *,int = 0) const override;

            protected:
                SegmentInformation *parent;
        };
    }
}

#endif /* SEGMENTBASE_H_ */
