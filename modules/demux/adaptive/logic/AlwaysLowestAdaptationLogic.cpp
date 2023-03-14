/*
 * AlwaysLowestAdaptationLogic.cpp
 *****************************************************************************
 * Copyright (C) 2014 - VideoLAN and VLC authors
  *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "AlwaysLowestAdaptationLogic.hpp"
#include "Representationselectors.hpp"

using namespace adaptive::logic;
using namespace adaptive::playlist;

AlwaysLowestAdaptationLogic::AlwaysLowestAdaptationLogic(vlc_object_t *obj):
    AbstractAdaptationLogic(obj)
{
}

BaseRepresentation *AlwaysLowestAdaptationLogic::getNextRepresentation(BaseAdaptationSet *adaptSet, BaseRepresentation *)
{
    RepresentationSelector selector(maxwidth, maxheight);
    return selector.lowest(adaptSet);
}
