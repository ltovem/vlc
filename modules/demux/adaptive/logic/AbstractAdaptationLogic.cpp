// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * AbstractAdaptationLogic.cpp
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "AbstractAdaptationLogic.h"

#include <limits>

using namespace adaptive::logic;

AbstractAdaptationLogic::AbstractAdaptationLogic    (vlc_object_t *obj)
{
    p_obj = obj;
    maxwidth = std::numeric_limits<int>::max();
    maxheight = std::numeric_limits<int>::max();
}

AbstractAdaptationLogic::~AbstractAdaptationLogic   ()
{
}

void AbstractAdaptationLogic::updateDownloadRate    (const adaptive::ID &, size_t,
                                                     vlc_tick_t, vlc_tick_t)
{
}

void AbstractAdaptationLogic::setMaxDeviceResolution (int w, int h)
{
    maxwidth = (w > 0) ? w : std::numeric_limits<int>::max();
    maxheight = (h > 0) ? h : std::numeric_limits<int>::max();
}
