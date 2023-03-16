// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * AlwaysBestAdaptationLogic.cpp
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "AlwaysBestAdaptationLogic.h"
#include "Representationselectors.hpp"

using namespace adaptive::logic;
using namespace adaptive::playlist;

AlwaysBestAdaptationLogic::AlwaysBestAdaptationLogic    (vlc_object_t *obj) :
                           AbstractAdaptationLogic      (obj)
{
}

BaseRepresentation *AlwaysBestAdaptationLogic::getNextRepresentation(BaseAdaptationSet *adaptSet, BaseRepresentation *)
{
    RepresentationSelector selector(maxwidth, maxheight);
    return selector.highest(adaptSet);
}
