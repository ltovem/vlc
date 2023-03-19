// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Representationselectors.cpp
 *****************************************************************************
 Copyright (C) 2014 - VideoLAN and VLC authors
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "Representationselectors.hpp"
#include "../playlist/BaseRepresentation.h"
#include "../playlist/BaseAdaptationSet.h"

#include <algorithm>
#include <limits>

using namespace adaptive::logic;

RepresentationSelector::RepresentationSelector(int maxwidth, int maxheight)
{
    this->maxwidth = maxwidth;
    this->maxheight = maxheight;
}

RepresentationSelector::~RepresentationSelector()
{
}

BaseRepresentation * RepresentationSelector::lowest(BaseAdaptationSet *adaptSet) const
{
    const std::vector<BaseRepresentation *> &reps = adaptSet->getRepresentations();
    return (reps.empty()) ? nullptr : *(reps.begin());
}

BaseRepresentation * RepresentationSelector::highest(BaseAdaptationSet *adaptSet) const
{
    const std::vector<BaseRepresentation *> &reps = adaptSet->getRepresentations();

    for(auto it=reps.crbegin(); it!=reps.crend(); ++it)
    {
        if( (*it)->getWidth() <= maxwidth && (*it)->getHeight() <= maxheight )
            return *it;
    }
    return lowest(adaptSet);
}

BaseRepresentation * RepresentationSelector::higher(BaseAdaptationSet *adaptSet, BaseRepresentation *rep) const
{
    const std::vector<BaseRepresentation *> &reps = adaptSet->getRepresentations();
    auto it = std::upper_bound(reps.cbegin(), reps.cend(), rep, BaseRepresentation::bwCompare);
    BaseRepresentation *upperRep = (it == reps.end()) ? rep : *it;
    if( upperRep->getWidth() > maxwidth || upperRep->getHeight() > maxheight )
        upperRep = rep;
    return upperRep;
}

BaseRepresentation * RepresentationSelector::lower(BaseAdaptationSet *adaptSet, BaseRepresentation *rep) const
{
    const std::vector<BaseRepresentation *> &reps = adaptSet->getRepresentations();
    auto it = std::lower_bound(reps.cbegin(), reps.cend(), rep, BaseRepresentation::bwCompare);
    return (it > reps.cbegin()) ? *(--it) : rep;
}

BaseRepresentation * RepresentationSelector::select(BaseAdaptationSet *adaptSet) const
{
    return select(adaptSet, std::numeric_limits<uint64_t>::max());
}
BaseRepresentation * RepresentationSelector::select(BaseAdaptationSet *adaptSet, uint64_t bitrate) const
{
    if (adaptSet == nullptr)
        return nullptr;

    const std::vector<BaseRepresentation *> &reps = adaptSet->getRepresentations();
    return select(reps, 0, bitrate);
}

BaseRepresentation * RepresentationSelector::select(const std::vector<BaseRepresentation *>& reps,
                                                uint64_t minbitrate, uint64_t maxbitrate) const
{
    BaseRepresentation  *candidate = nullptr, *lowest = nullptr;
    std::vector<BaseRepresentation *>::const_iterator repIt;
    for(repIt=reps.begin(); repIt!=reps.end(); ++repIt)
    {
        if ( !lowest || (*repIt)->getBandwidth() < lowest->getBandwidth())
            lowest = *repIt;

        if( (*repIt)->getWidth() > maxwidth || (*repIt)->getHeight() > maxheight )
            continue;

        if ( (*repIt)->getBandwidth() < maxbitrate &&
             (*repIt)->getBandwidth() > minbitrate )
        {
            candidate = (*repIt);
            minbitrate = (*repIt)->getBandwidth();
        }
    }

    if (candidate == nullptr)
        return lowest;

    return candidate;
}
