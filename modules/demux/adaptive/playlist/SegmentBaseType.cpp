/*
 * SegmentBaseType.cpp
 *****************************************************************************
 * Copyright (C) 2020 VideoLabs, VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "SegmentBaseType.hpp"
#include "SegmentInformation.hpp"
#include "SegmentTemplate.h"
#include "SegmentTimeline.h"

#include <limits>

using namespace adaptive::playlist;

Segment * AbstractSegmentBaseType::findSegmentByScaledTime(const std::vector<Segment *> &segments,
                                                                 stime_t time)
{
    if(segments.empty() || (segments.size() > 1 && segments[1]->startTime.Get() == 0) )
        return nullptr;

    Segment *ret = nullptr;
    std::vector<Segment *>::const_iterator it = segments.begin();
    while(it != segments.end())
    {
        Segment *seg = *it;
        if(seg->startTime.Get() > time)
        {
            if(it == segments.begin())
                return nullptr;
            else
                break;
        }

        ret = seg;
        it++;
    }

    return ret;
}

uint64_t AbstractSegmentBaseType::findSegmentNumberByScaledTime(const std::vector<Segment *> &segments,
                                                               stime_t time)
{
    Segment *s = findSegmentByScaledTime(segments, time);
    if(!s)
        return std::numeric_limits<uint64_t>::max();
    return s->getSequenceNumber();
}

AbstractSegmentBaseType::AbstractSegmentBaseType(SegmentInformation *parent, AttrsNode::Type t)
                : AttrsNode(t, parent)
{
    this->parent = parent;
}

AbstractSegmentBaseType::~AbstractSegmentBaseType()
{
}

InitSegment *AbstractSegmentBaseType::getInitSegment() const
{
    return initialisationSegment.Get();
}

IndexSegment *AbstractSegmentBaseType::getIndexSegment() const
{
    return indexSegment.Get();
}

void AbstractSegmentBaseType::debug(vlc_object_t *obj, int indent) const
{
    if(initialisationSegment.Get())
        initialisationSegment.Get()->debug(obj, indent);
    if(indexSegment.Get())
        indexSegment.Get()->debug(obj, indent);
}

AbstractMultipleSegmentBaseType::AbstractMultipleSegmentBaseType(SegmentInformation *parent,
                                                                 AttrsNode::Type type)
                : AbstractSegmentBaseType(parent, type)
{
}

AbstractMultipleSegmentBaseType::~AbstractMultipleSegmentBaseType()
{
}

void AbstractMultipleSegmentBaseType::updateWith(AbstractMultipleSegmentBaseType *updated,
                                                 bool)
{
    SegmentTimeline *local = static_cast<SegmentTimeline *>(getAttribute(Type::Timeline));
    SegmentTimeline *other = static_cast<SegmentTimeline *>(updated->getAttribute(Type::Timeline));
    if(local && other)
        local->updateWith(*other);
}
