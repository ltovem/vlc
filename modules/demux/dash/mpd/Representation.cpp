/*
 * Representation.cpp
 *****************************************************************************
 * Copyright (C) 2010 - 2011 Klagenfurt University
 *
 * Created on: Aug 10, 2010
 * Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
 *          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>

#include "Representation.h"
#include "AdaptationSet.h"
#include "MPD.h"
#include "../../adaptive/playlist/SegmentTemplate.h"
#include "../../adaptive/playlist/SegmentTimeline.h"
#include "TemplatedUri.hpp"

using namespace dash::mpd;

Representation::Representation  ( AdaptationSet *set ) :
                BaseRepresentation( set )
{
}

Representation::~Representation ()
{
}

StreamFormat Representation::getStreamFormat() const
{
    return StreamFormat(getMimeType());
}

std::string Representation::contextualize(size_t number, const std::string &component,
                                          const SegmentTemplate *templ) const
{
    std::string str(component);
    if(!templ)
        return str;

    std::string::size_type pos = 0;
    while(pos < str.length())
    {
        TemplatedUri::Token token;
        if(str[pos] == '$' && TemplatedUri::IsDASHToken(str, pos, token))
        {
            TemplatedUri::TokenReplacement replparam;
            switch(token.type)
            {
                case TemplatedUri::Token::TOKEN_TIME:
                    replparam.value = templ ? getScaledTimeBySegmentNumber(number, templ) : 0;
                    break;
                case TemplatedUri::Token::TOKEN_BANDWIDTH:
                    replparam.value = getBandwidth();
                    break;
                case TemplatedUri::Token::TOKEN_NUMBER:
                    replparam.value = number;
                    break;
                case TemplatedUri::Token::TOKEN_REPRESENTATION:
                    replparam.str = id.str();
                    break;
                case TemplatedUri::Token::TOKEN_ESCAPE:
                    break;

                default:
                    pos += token.fulllength;
                    continue;
            }
            /* Replace with newvalue */
            std::string::size_type newlen = TemplatedUri::ReplaceDASHToken(
                                                    str, pos, token, replparam);
            if(newlen == std::string::npos)
                newlen = token.fulllength;
            pos += newlen;
        }
        else pos++;
    }

    return str;
}

stime_t Representation::getScaledTimeBySegmentNumber(uint64_t index, const SegmentTemplate *templ) const
{
    stime_t time = 0;
    const SegmentTimeline *tl = templ->inheritSegmentTimeline();
    if(tl)
    {
        time = tl->getScaledPlaybackTimeByElementNumber(index);
    }
    else if(templ->inheritDuration())
    {
        time = templ->inheritDuration() * index;
    }
    return time;
}

