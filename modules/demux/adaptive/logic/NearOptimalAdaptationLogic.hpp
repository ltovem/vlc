// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * NearOptimalAdaptationLogic.hpp
 *****************************************************************************
 Copyright (C) 2017 - VideoLAN Authors
 *****************************************************************************/
#ifndef NEAROPTIMALADAPTATIONLOGIC_HPP
#define NEAROPTIMALADAPTATIONLOGIC_HPP

#include "AbstractAdaptationLogic.h"
#include "Representationselectors.hpp"
#include "../tools/MovingAverage.hpp"
#include <map>

namespace adaptive
{
    namespace logic
    {
        class NearOptimalContext
        {
            friend class NearOptimalAdaptationLogic;

            public:
                NearOptimalContext();

            private:
                vlc_tick_t buffering_min;
                vlc_tick_t buffering_level;
                vlc_tick_t buffering_target;
                unsigned last_download_rate;
                MovingAverage<unsigned> average;
        };

        class NearOptimalAdaptationLogic : public AbstractAdaptationLogic
        {
            public:
                NearOptimalAdaptationLogic(vlc_object_t *);
                virtual ~NearOptimalAdaptationLogic();

                virtual BaseRepresentation* getNextRepresentation(BaseAdaptationSet *,
                                                                  BaseRepresentation *) override;
                virtual void                updateDownloadRate     (const ID &, size_t,
                                                                    vlc_tick_t, vlc_tick_t) override;
                virtual void                trackerEvent           (const TrackerEvent &) override;

            private:
                BaseRepresentation *        getNextQualityIndex( BaseAdaptationSet *, RepresentationSelector &,
                                                                 float gammaP, float VD,
                                                                 float Q /*current buffer level*/);
                float                       getUtility(const BaseRepresentation *);
                unsigned                    getAvailableBw(unsigned, const BaseRepresentation *) const;
                unsigned                    getMaxCurrentBw() const;
                std::map<adaptive::ID, NearOptimalContext> streams;
                std::map<uint64_t, float>   utilities;
                unsigned                    currentBps;
                unsigned                    usedBps;
                vlc_mutex_t                 lock;
        };
    }
}

#endif // NEAROPTIMALADAPTATIONLOGIC_HPP
