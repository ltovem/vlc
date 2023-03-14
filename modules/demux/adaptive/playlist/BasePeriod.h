/*
 * Period.h
 *****************************************************************************
 * Copyright (C) 2010 - 2011 Klagenfurt University
 *
 * Created on: Aug 10, 2010
 * Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
 *          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef BASEPERIOD_H_
#define BASEPERIOD_H_

#include <vector>

#include "BaseAdaptationSet.h"
#include "SegmentInformation.hpp"
#include "../tools/Properties.hpp"

namespace adaptive
{
    namespace playlist
    {
        class BasePeriod : public SegmentInformation
        {
            public:
                BasePeriod(BasePlaylist *);
                virtual ~BasePeriod ();

                const std::vector<BaseAdaptationSet *>& getAdaptationSets   () const;
                BaseAdaptationSet *                 getAdaptationSetByID(const ID &) const;
                void                                addAdaptationSet    (BaseAdaptationSet *AdaptationSet);
                void                                debug               (vlc_object_t *,int = 0) const;

                virtual vlc_tick_t getPeriodStart() const override;
                virtual vlc_tick_t getPeriodDuration() const override;
                virtual BasePlaylist *getPlaylist() const override;

                Property<vlc_tick_t> duration;
                Property<vlc_tick_t> startTime;

            private:
                std::vector<BaseAdaptationSet *>    adaptationSets;
                BasePlaylist *playlist;
        };
    }
}

#endif /* BASEPERIOD_H_ */
