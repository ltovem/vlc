// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * AbstractAdaptationLogic.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef ABSTRACTADAPTATIONLOGIC_H_
#define ABSTRACTADAPTATIONLOGIC_H_

#include "IDownloadRateObserver.h"
#include "../SegmentTracker.hpp"

namespace adaptive
{
    namespace playlist
    {
        class BaseRepresentation;
        class BaseAdaptationSet;
    }

    namespace logic
    {
        using namespace playlist;

        class AbstractAdaptationLogic : public IDownloadRateObserver,
                                        public SegmentTrackerListenerInterface
        {
            public:
                AbstractAdaptationLogic             (vlc_object_t *);
                virtual ~AbstractAdaptationLogic    ();

                virtual BaseRepresentation* getNextRepresentation(BaseAdaptationSet *, BaseRepresentation *) = 0;
                virtual void                updateDownloadRate     (const ID &, size_t,
                                                                    vlc_tick_t, vlc_tick_t) override;
                virtual void                trackerEvent           (const TrackerEvent &) override {}
                void                        setMaxDeviceResolution (int, int);

                enum class LogicType
                {
                    Default = 0,
                    AlwaysBest,
                    AlwaysLowest,
                    RateBased,
                    FixedRate,
                    Predictive,
                    NearOptimal,
                };

            protected:
                vlc_object_t *p_obj;
                int maxwidth;
                int maxheight;
        };
    }
}

#endif /* ABSTRACTADAPTATIONLOGIC_H_ */
