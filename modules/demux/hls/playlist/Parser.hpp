// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Parser.hpp
 *****************************************************************************
 Copyright © 2015 - VideoLAN and VLC Authors
 *****************************************************************************/
#ifndef PARSER_HPP
#define PARSER_HPP

#include "../../adaptive/playlist/SegmentBaseType.hpp"

#include <cstdlib>
#include <sstream>
#include <list>

#include <vlc_common.h>

namespace adaptive
{
    class SharedResources;

    namespace playlist
    {
        class SegmentInformation;
        class SegmentTemplate;
        class BaseAdaptationSet;
    }
}

namespace hls
{
    namespace playlist
    {
        using namespace adaptive::playlist;

        class M3U8;
        class AttributesTag;
        class Tag;
        class HLSRepresentation;

        class M3U8Parser
        {
            public:
                M3U8Parser             (adaptive::SharedResources *);
                virtual ~M3U8Parser    ();

                M3U8 *             parse  (vlc_object_t *p_obj, stream_t *p_stream, const std::string &);
                bool appendSegmentsFromPlaylistURI(vlc_object_t *, HLSRepresentation *);

            private:
                HLSRepresentation * createRepresentation(BaseAdaptationSet *, const AttributesTag *);
                void createAndFillRepresentation(vlc_object_t *, BaseAdaptationSet *,
                                                 const AttributesTag *, const std::list<Tag *>&);
                void fillRepresentationFromMediainfo(const AttributesTag *, const std::string &,
                                                     HLSRepresentation *);
                void fillAdaptsetFromMediainfo(const AttributesTag *, const std::string &,
                                               const std::string &, BaseAdaptationSet *);
                void parseSegments(vlc_object_t *, HLSRepresentation *, const std::list<Tag *>&);
                std::list<Tag *> parseEntries(stream_t *);
                adaptive::SharedResources *resources;
        };
    }
}

#endif // PARSER_HPP
