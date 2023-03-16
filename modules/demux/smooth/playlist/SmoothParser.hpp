// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Parser.hpp
 *****************************************************************************
 Copyright © 2015 - VideoLAN and VLC Authors
 *****************************************************************************/
#ifndef MANIFESTPARSER_HPP
#define MANIFESTPARSER_HPP

#include "../../adaptive/playlist/SegmentBaseType.hpp"

#include <cstdlib>
#include <sstream>

#include <vlc_common.h>

namespace adaptive
{
    namespace playlist
    {
        class SegmentInformation;
        class SegmentTemplate;
        class BaseAdaptationSet;
    }
    namespace xml
    {
        class Node;
    }
}

namespace smooth
{
    namespace playlist
    {
        using namespace adaptive::playlist;
        using namespace adaptive;

        class Manifest;

        class ManifestParser
        {
            public:
                ManifestParser             (xml::Node *, vlc_object_t *,
                                            stream_t *, const std::string &);
                virtual ~ManifestParser    ();

                Manifest * parse();

            private:
                xml::Node       *root;
                vlc_object_t    *p_object;
                stream_t        *p_stream;
                std::string      playlisturl;
        };
    }
}


#endif // MANIFESTPARSER_HPP
