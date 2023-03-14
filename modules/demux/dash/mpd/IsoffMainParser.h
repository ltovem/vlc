/*
 * IsoffMainParser.h
 *****************************************************************************
 * Copyright (C) 2010 - 2012 Klagenfurt University
 *
 * Created on: Jan 27, 2012
 * Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
 *          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef ISOFFMAINPARSER_H_
#define ISOFFMAINPARSER_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "../../adaptive/playlist/SegmentBaseType.hpp"
#include "Profile.hpp"

#include <cstdlib>

#include <vlc_common.h>

namespace adaptive
{
    namespace playlist
    {
        class SegmentInformation;
        class SegmentTemplate;
        class BasePeriod;
        class CommonAttributesElements;
    }
    namespace xml
    {
        class Node;
    }
}

namespace dash
{
    namespace mpd
    {
        class AdaptationSet;
        class MPD;

        using namespace adaptive::playlist;
        using namespace adaptive;

        class IsoffMainParser
        {
            public:
                IsoffMainParser             (xml::Node *root, vlc_object_t *p_object,
                                             stream_t *p_stream, const std::string &);
                virtual ~IsoffMainParser    ();
                MPD *   parse();

            private:
                mpd::Profile getProfile     () const;
                void    parseMPDBaseUrl     (MPD *, xml::Node *);
                void    parseMPDAttributes  (MPD *, xml::Node *);
                void    parseAdaptationSets (MPD *, xml::Node *periodNode, BasePeriod *period);
                void    parseRepresentations(MPD *, xml::Node *adaptationSetNode, AdaptationSet *adaptationSet);
                void    parseInitSegment    (xml::Node *, Initializable<InitSegment> *, SegmentInformation *);
                void    parseTimeline       (xml::Node *, AbstractMultipleSegmentBaseType *);
                void    parsePeriods        (MPD *, xml::Node *);
                size_t  parseSegmentInformation(MPD *, xml::Node *, SegmentInformation *, uint64_t *);
                size_t  parseSegmentBase    (MPD *, xml::Node *, SegmentInformation *);
                size_t  parseSegmentList    (MPD *, xml::Node *, SegmentInformation *);
                size_t  parseSegmentTemplate(MPD *, xml::Node *, SegmentInformation *);
                void    parseProgramInformation(xml::Node *, MPD *);
                void    parseSegmentBaseType(MPD *mpd, xml::Node *node,
                                             AbstractSegmentBaseType *base,
                                             SegmentInformation *parent);
                void    parseMultipleSegmentBaseType(MPD *mpd, xml::Node *node,
                                             AbstractMultipleSegmentBaseType *base,
                                             SegmentInformation *parent);
                void    parseCommonAttributesElements(xml::Node *, CommonAttributesElements *);

                xml::Node       *root;
                vlc_object_t    *p_object;
                stream_t        *p_stream;
                std::string      playlisturl;
        };
    }
}

#endif /* ISOFFMAINPARSER_H_ */
