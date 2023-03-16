// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * AdaptationSet.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef ADAPTATIONSET_H_
#define ADAPTATIONSET_H_

#include "../../adaptive/playlist/BaseAdaptationSet.h"
#include "DASHCommonAttributesElements.h"

namespace dash
{
    namespace mpd
    {
        class Period;
        class Representation;

        using namespace adaptive;

        class AdaptationSet : public adaptive::playlist::BaseAdaptationSet,
                              public DASHCommonAttributesElements
        {
            public:
                AdaptationSet(playlist::BasePeriod *);
                virtual ~AdaptationSet();

                virtual StreamFormat            getStreamFormat() const override;
                bool                            getSubsegmentAlignmentFlag() const;
                void                            setSubsegmentAlignmentFlag( bool alignment );

            private:
                bool                            subsegmentAlignmentFlag;
        };
    }
}

#endif /* ADAPTATIONSET_H_ */
