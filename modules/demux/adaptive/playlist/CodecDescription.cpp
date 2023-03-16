// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * CodecDescription.cpp
 *****************************************************************************
 Copyright (C) 2021 - VideoLabs, VideoLAN and VLC Authors
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_es.h>

#include "CodecDescription.hpp"
#include "../tools/FormatNamespace.hpp"

using namespace adaptive::playlist;

CodecDescription::CodecDescription()
{
    es_format_Init(&fmt, UNKNOWN_ES, 0);
}

CodecDescription::CodecDescription(const std::string &codec)
{
    FormatNamespace fnsp(codec);
    es_format_Init(&fmt, fnsp.getFmt()->i_cat, fnsp.getFmt()->i_codec);
    es_format_Copy(&fmt, fnsp.getFmt());
}

CodecDescription::~CodecDescription()
{
    es_format_Clean(&fmt);
}

const es_format_t * CodecDescription::getFmt() const
{
    return &fmt;
}

void CodecDescription::setDimensions(unsigned w, unsigned h)
{
    if(fmt.i_cat != VIDEO_ES || !w || !h)
        return;
    fmt.video.i_visible_width = w;
    fmt.video.i_visible_height = h;
}

void CodecDescription::setDescription(const std::string &d)
{
    free(fmt.psz_description);
    fmt.psz_description = ::strdup(d.c_str());
}

void CodecDescription::setLanguage(const std::string &l)
{
    free(fmt.psz_language);
    fmt.psz_language = ::strdup(l.c_str());
}

void CodecDescription::setAspectRatio(const AspectRatio &r)
{
    if(fmt.i_cat != VIDEO_ES || !r.isValid())
        return;
    fmt.video.i_sar_num = r.num();
    fmt.video.i_sar_den = r.den();
}

void CodecDescription::setFrameRate(const Rate &r)
{
    if(fmt.i_cat != VIDEO_ES || !r.isValid())
        return;
    fmt.video.i_frame_rate = r.num();
    fmt.video.i_frame_rate_base = r.den();
}

void CodecDescription::setSampleRate(const Rate &r)
{
    if(fmt.i_cat != AUDIO_ES || !r.isValid())
        return;
    fmt.audio.i_rate = r.num();
}

void CodecDescription::setChannelsCount(unsigned c)
{
    fmt.audio.i_channels = c;
}

CodecDescriptionList::CodecDescriptionList()
{

}

CodecDescriptionList::~CodecDescriptionList()
{
    while(!empty())
    {
        delete front();
        pop_front();
    }
}
