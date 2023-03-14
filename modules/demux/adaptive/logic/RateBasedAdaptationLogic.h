/*
 * RateBasedAdaptationLogic.h
 *****************************************************************************
 * Copyright (C) 2010 - 2011 Klagenfurt University
 *
 * Created on: Aug 10, 2010
 * Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
 *          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef RATEBASEDADAPTATIONLOGIC_H_
#define RATEBASEDADAPTATIONLOGIC_H_

#include "AbstractAdaptationLogic.h"
#include "../tools/MovingAverage.hpp"

namespace adaptive
{
    namespace logic
    {

        class RateBasedAdaptationLogic : public AbstractAdaptationLogic
        {
            public:
                RateBasedAdaptationLogic            (vlc_object_t *);
                virtual ~RateBasedAdaptationLogic   ();

                BaseRepresentation *getNextRepresentation(BaseAdaptationSet *,
                                                          BaseRepresentation *) override;
                virtual void updateDownloadRate(const ID &, size_t,
                                                vlc_tick_t, vlc_tick_t) override;
                virtual void trackerEvent(const TrackerEvent &) override;

            private:
                size_t                  bpsAvg;
                size_t                  currentBps;
                size_t                  usedBps;

                MovingAverage<size_t>   average;

                size_t                  dlsize;
                vlc_tick_t              dllength;

                mutable vlc_mutex_t     lock;
        };

        class FixedRateAdaptationLogic : public AbstractAdaptationLogic
        {
            public:
                FixedRateAdaptationLogic(vlc_object_t *, size_t);

                BaseRepresentation *getNextRepresentation(BaseAdaptationSet *,
                                                          BaseRepresentation *) override;

            private:
                size_t                  currentBps;
        };
    }
}

#endif /* RATEBASEDADAPTATIONLOGIC_H_ */
