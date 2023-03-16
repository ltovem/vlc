// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * DASHStream.cpp
 *****************************************************************************
 Copyright (C) 2015 - VideoLAN and VLC authors
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "DASHStream.hpp"

using namespace dash;

DASHStream::DASHStream(demux_t *demux)
    :AbstractStream(demux)
{
}

AbstractDemuxer *DASHStream::newDemux(vlc_object_t *p_obj, const StreamFormat &format,
                                      es_out_t *out, AbstractSourceStream *source) const
{
    AbstractDemuxer *ret = nullptr;
    switch(format)
    {
        case StreamFormat::Type::MP4:
        case StreamFormat::Type::MPEG2TS:
            ret = AbstractStream::newDemux(p_obj, format, out, source);
            break;

        case StreamFormat::Type::WebM:
            ret = new Demuxer(p_obj, "mkv_trusted", out, source);
            break;

        case StreamFormat::Type::WebVTT:
            ret = new SlaveDemuxer(p_obj, "webvtt", out, source);
            break;

        case StreamFormat::Type::TTML:
            ret = new SlaveDemuxer(p_obj, "ttml", out, source);
            break;

        default:
        case StreamFormat::Type::Unsupported:
            break;
    }

    return ret;
}

AbstractStream * DASHStreamFactory::create(demux_t *realdemux, const StreamFormat &format,
                                   SegmentTracker *tracker) const
{
    AbstractStream *stream = new (std::nothrow) DASHStream(realdemux);
    if(stream && !stream->init(format, tracker))
    {
        delete stream;
        return nullptr;
    }
    return stream;
}
