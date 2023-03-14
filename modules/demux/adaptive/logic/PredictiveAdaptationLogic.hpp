/*
 * PredictiveAdaptationLogic.hpp
 *****************************************************************************
 * Copyright (C) 2016 - VideoLAN Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef PREDICTIVEADAPTATIONLOGIC_HPP
#define PREDICTIVEADAPTATIONLOGIC_HPP

#include "AbstractAdaptationLogic.h"
#include "../tools/MovingAverage.hpp"
#include <map>

namespace adaptive
{
    namespace logic
    {
        class PredictiveStats
        {
            friend class PredictiveAdaptationLogic;

            public:
                PredictiveStats();
                bool starting() const;

            private:
                size_t  segments_count;
                vlc_tick_t buffering_level;
                vlc_tick_t buffering_target;
                unsigned last_download_rate;
                vlc_tick_t last_duration;
                MovingAverage<unsigned> average;
        };

        class PredictiveAdaptationLogic : public AbstractAdaptationLogic
        {
            public:
                PredictiveAdaptationLogic(vlc_object_t *);
                virtual ~PredictiveAdaptationLogic();

                virtual BaseRepresentation* getNextRepresentation(BaseAdaptationSet *,
                                                                  BaseRepresentation *) override;
                virtual void                updateDownloadRate     (const ID &, size_t,
                                                                    vlc_tick_t, vlc_tick_t) override;
                virtual void                trackerEvent           (const TrackerEvent &) override;

            private:
                unsigned                    getAvailableBw(unsigned, const BaseRepresentation *) const;
                std::map<adaptive::ID, PredictiveStats> streams;
                unsigned                    usedBps;
                vlc_mutex_t                 lock;
        };
    }
}

#endif // PREDICTIVEADAPTATIONLOGIC_HPP
