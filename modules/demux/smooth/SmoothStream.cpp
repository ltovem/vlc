// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * SmoothStream.cpp
 *****************************************************************************
 Copyright (C) 2015 - VideoLAN and VLC authors
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "SmoothStream.hpp"
#include <vlc_demux.h>

using namespace smooth;

SmoothStream::SmoothStream(demux_t *demux)
    :AbstractStream(demux)
{
    mightalwaysstartfromzero = true;
}

AbstractDemuxer *SmoothStream::newDemux(vlc_object_t *p_obj, const StreamFormat &format,
                                        es_out_t *out, AbstractSourceStream *source) const
{
    if(format != StreamFormat::Type::MP4)
        return nullptr;
    return AbstractStream::newDemux(p_obj, format, out, source);
}

AbstractStream * SmoothStreamFactory::create(demux_t *realdemux, const StreamFormat &format,
                                             SegmentTracker *tracker) const
{
    SmoothStream *stream = new (std::nothrow) SmoothStream(realdemux);
    if(stream && !stream->init(format,tracker))
    {
        delete stream;
        return nullptr;
    }
    return stream;
}
