/*
 * AdaptationSet.cpp
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

#include "AdaptationSet.h"
#include "Representation.h"
#include "MPD.h"

using namespace dash::mpd;
using namespace adaptive::playlist;

AdaptationSet::AdaptationSet(BasePeriod *period) :
    BaseAdaptationSet( period ),
    DASHCommonAttributesElements(),
    subsegmentAlignmentFlag( false )
{
}

AdaptationSet::~AdaptationSet()
{
}

StreamFormat AdaptationSet::getStreamFormat() const
{
    if(!getMimeType().empty())
        return StreamFormat(getMimeType());
    else
        return BaseAdaptationSet::getStreamFormat();
}

bool AdaptationSet::getSubsegmentAlignmentFlag() const
{
    return subsegmentAlignmentFlag;
}

void AdaptationSet::setSubsegmentAlignmentFlag(bool alignment)
{
    subsegmentAlignmentFlag = alignment;
}


