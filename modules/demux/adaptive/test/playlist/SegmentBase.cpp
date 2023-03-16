// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 *
 *****************************************************************************
 * Copyright (C) 2020 VideoLabs, VideoLAN and VLC Authors
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "../../playlist/Segment.h"
#include "../../playlist/SegmentBase.h"
#include "../../playlist/BaseAdaptationSet.h"
#include "../../playlist/BaseRepresentation.h"
#include "../../playlist/SegmentInformation.hpp"

#include "../test.hpp"

#include <limits>

using namespace adaptive;
using namespace adaptive::playlist;

using SplitPoint = SegmentInformation::SplitPoint;

int SegmentBase_test()
{
    BaseRepresentation *rep = new BaseRepresentation(nullptr);
    try
    {
        SegmentBase *segmentBase = new SegmentBase(nullptr);
        Timescale timescale(100);
        segmentBase->addAttribute(new TimescaleAttr(timescale));
        rep->addAttribute(segmentBase);
        /* Check failures */

        Expect(segmentBase->getMediaSegment(0) == nullptr);
        uint64_t number; bool discont;
        Expect(segmentBase->getSegmentNumberByTime(1, &number) == false);
        Expect(segmentBase->getNextMediaSegment(0, &number, &discont) == nullptr);
        vlc_tick_t time, duration;
        Expect(segmentBase->getPlaybackTimeDurationBySegmentNumber(0, &time, &duration) == false);

        /* Create a split range */
        std::vector<SplitPoint> splitlist;
        for(int i=0; i<10; i++)
        {
            SplitPoint point;
            point.time = i * 100;
            point.duration = 100;
            point.offset = 123 + i * 100;
            splitlist.push_back(point);
        }
        rep->SplitUsingIndex(splitlist);

        /* For now we can't tell anything without main segment byte range */
        Expect(segmentBase->getMediaSegment(0) == nullptr);

        segmentBase->setByteRange(111, 2000);

        segmentBase->duration.Set(100 * 10);
        rep->SplitUsingIndex(splitlist);
        Expect(segmentBase->subSegments().size());
        Expect(segmentBase->getMediaSegment(0) != nullptr);
        Expect(segmentBase->getMinAheadTime(0) == timescale.ToTime(9 * 100));
        Expect(segmentBase->getSegmentNumberByTime(timescale.ToTime(9 * 100 - 1), &number));
        Expect(number == 8);
        Expect(segmentBase->getMinAheadTime(7) == timescale.ToTime(2 * 100));
        Expect(segmentBase->getPlaybackTimeDurationBySegmentNumber(7, &time, &duration) == true);
        Expect(time == timescale.ToTime(7 * 100));
        Expect(duration == timescale.ToTime(100));
        Segment *seg = segmentBase->getMediaSegment(7);
        Expect(seg);
        Expect(seg->getSequenceNumber() == 7);
        Expect(seg->getOffset() == 123 + 7*100);

        seg = segmentBase->getNextMediaSegment(7, &number, &discont);
        Expect(seg);
        Expect(seg->getSequenceNumber() == 7);
        Expect(number == 7);
        Expect(!discont);

        delete rep;

    } catch (...) {
        delete rep;
        return 1;
    }

    return 0;
}
