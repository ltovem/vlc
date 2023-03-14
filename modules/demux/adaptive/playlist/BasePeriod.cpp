/*
 * BasePeriod.cpp
 *****************************************************************************
 * Copyright (C) 2010 - 2011 Klagenfurt University
 *
 * Created on: Aug 10, 2010
 * Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
 *          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "BasePeriod.h"
#include "BasePlaylist.hpp"
#include "SegmentBaseType.hpp"
#include "../Streams.hpp"

#include <vlc_common.h>
#include <vlc_arrays.h>
#include <algorithm>
#include <cassert>

using namespace adaptive::playlist;

BasePeriod::BasePeriod(BasePlaylist *playlist_) :
    SegmentInformation( playlist_ )
{
    duration.Set(0);
    startTime.Set(0);
    playlist = playlist_;
}

BasePeriod::~BasePeriod ()
{
    vlc_delete_all( adaptationSets );
    childs.clear();
}

BasePlaylist *BasePeriod::getPlaylist() const
{
    return playlist;
}

const std::vector<BaseAdaptationSet*>&  BasePeriod::getAdaptationSets() const
{
    return adaptationSets;
}

void BasePeriod::addAdaptationSet(BaseAdaptationSet *adaptationSet)
{
    auto p = std::find_if(adaptationSets.begin(), adaptationSets.end(),
        [adaptationSet](BaseAdaptationSet *s){
            return adaptationSet->getRole() < s->getRole(); });
    adaptationSets.insert(p, adaptationSet);
    childs.push_back(adaptationSet);
}

BaseAdaptationSet *BasePeriod::getAdaptationSetByID(const adaptive::ID &id) const
{
    for(auto it = adaptationSets.cbegin(); it!= adaptationSets.cend(); ++it)
    {
        if( (*it)->getID() == id )
            return *it;
    }
    return nullptr;
}

void BasePeriod::debug(vlc_object_t *obj, int indent) const
{
    std::string text(indent, ' ');
    text.append("Period");
    msg_Dbg(obj, "%s", text.c_str());
    const AbstractSegmentBaseType *profile = getProfile();
    if(profile)
        profile->debug(obj, indent + 1);
    std::vector<BaseAdaptationSet *>::const_iterator k;
    for(k = adaptationSets.begin(); k != adaptationSets.end(); ++k)
        (*k)->debug(obj, indent + 1);
}

vlc_tick_t BasePeriod::getPeriodStart() const
{
    return startTime.Get();
}

vlc_tick_t BasePeriod::getPeriodDuration() const
{
    return duration.Get();
}
