/*
 * M3U8.cpp
 *****************************************************************************
 * Copyright © 2015 - VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "M3U8.hpp"
#include "HLSRepresentation.hpp"
#include "../../adaptive/playlist/BasePeriod.h"
#include "../../adaptive/playlist/BaseAdaptationSet.h"

using namespace hls::playlist;

M3U8::M3U8 (vlc_object_t *p_object) :
    BasePlaylist(p_object)
{
    minUpdatePeriod.Set( VLC_TICK_FROM_SEC(5) );
}

M3U8::~M3U8()
{
}

bool M3U8::isLive() const
{
    bool b_live = false;
    std::vector<BasePeriod *>::const_iterator itp;
    for(itp = periods.begin(); itp != periods.end(); ++itp)
    {
        const BasePeriod *period = *itp;
        const std::vector<BaseAdaptationSet *> &sets = period->getAdaptationSets();
        for(auto ita = sets.cbegin(); ita != sets.cend(); ++ita)
        {
            BaseAdaptationSet *adaptSet = *ita;
            const std::vector<BaseRepresentation *> &reps = adaptSet->getRepresentations();
            for(auto itr = reps.cbegin(); itr != reps.cend(); ++itr)
            {
                const HLSRepresentation *rep = dynamic_cast<const HLSRepresentation *>(*itr);
                if(rep->initialized())
                {
                    if(rep->isLive())
                        b_live = true;
                    else
                        return false; /* Any non live has higher priority */
                }
            }
        }
    }

    return b_live;
}

