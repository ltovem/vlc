/*
 * Representation.h
 *****************************************************************************
 * Copyright (C) 2010 - 2011 Klagenfurt University
 *
 * Created on: Aug 10, 2010
 * Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
 *          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef DASHREPRESENTATION_H_
#define DASHREPRESENTATION_H_

#include "DASHCommonAttributesElements.h"
#include "../../adaptive/playlist/SegmentBaseType.hpp"
#include "../../adaptive/playlist/BaseRepresentation.h"

namespace dash
{
    namespace mpd
    {
        class AdaptationSet;
        class MPD;

        using namespace adaptive;
        using namespace adaptive::playlist;

        class Representation : public BaseRepresentation,
                               public DASHCommonAttributesElements
        {
            public:
                Representation( AdaptationSet * );
                virtual ~Representation ();

                virtual StreamFormat getStreamFormat() const override;

                /* for segment templates */
                virtual std::string contextualize(size_t, const std::string &,
                                                  const SegmentTemplate *) const override;

            private:

                /* for contextualize() */
                stime_t getScaledTimeBySegmentNumber(uint64_t, const SegmentTemplate *) const;
        };
    }
}

#endif /* DASHREPRESENTATION_H_ */
