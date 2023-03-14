/*
 * Representationselectors.hpp
 *****************************************************************************
 * Copyright (C) 2014 - VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef REPRESENTATIONSELECTORS_HPP
#define REPRESENTATIONSELECTORS_HPP

#include <vector>
#include <vlc_common.h>

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

        class RepresentationSelector
        {
        public:
            RepresentationSelector(int, int);
             ~RepresentationSelector();
            BaseRepresentation * lowest(BaseAdaptationSet *) const;
            BaseRepresentation * highest(BaseAdaptationSet *) const;
            BaseRepresentation * higher(BaseAdaptationSet *, BaseRepresentation *) const;
            BaseRepresentation * lower(BaseAdaptationSet *, BaseRepresentation *) const;
            BaseRepresentation * select(BaseAdaptationSet *) const;
            BaseRepresentation * select(BaseAdaptationSet *, uint64_t bitrate) const;

        protected:
            int maxwidth;
            int maxheight;
            BaseRepresentation * select(const std::vector<BaseRepresentation *>&reps,
                                        uint64_t minbitrate, uint64_t maxbitrate) const;
        };

    }
}

#endif // REPRESENTATIONSELECTORS_HPP
