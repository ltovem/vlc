// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * ForgedInitSegment.hpp
 *****************************************************************************
 Copyright (C) 2015 - VideoLAN Authors
 *****************************************************************************/
#ifndef FORGEDINITSEGMENT_HPP
#define FORGEDINITSEGMENT_HPP

#include "../../adaptive/playlist/Segment.h"
#include "CodecParameters.hpp"

#include <vlc_es.h>
#include <vlc_codecs.h>

namespace smooth
{
    namespace playlist
    {
        using namespace adaptive;
        using namespace adaptive::playlist;
        using namespace adaptive::http;

        class CodecParameters;

        class ForgedInitSegment : public InitSegment
        {
            public:
                ForgedInitSegment(ICanonicalUrl *parent, const std::string &,
                                  uint64_t, vlc_tick_t);
                virtual ~ForgedInitSegment();
                virtual SegmentChunk* toChunk(SharedResources *, size_t,
                                              BaseRepresentation *) override;
                void setVideoSize(unsigned w, unsigned h);
                void setTrackID(unsigned);
                void setLanguage(const std::string &);

            private:
                block_t * buildMoovBox(const CodecParameters &);
                std::string type;
                std::string language;
                unsigned width, height;
                unsigned track_id;
                Timescale timescale;
        };
    }
}

#endif // FORGEDINITSEGMENT_HPP
