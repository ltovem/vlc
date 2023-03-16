// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * SmoothStream.hpp
 *****************************************************************************
 Copyright (C) 2015 - VideoLAN and VLC authors
 *****************************************************************************/
#ifndef SMOOTHSTREAM_HPP
#define SMOOTHSTREAM_HPP

#include "../adaptive/Streams.hpp"

namespace smooth
{
    using namespace adaptive;

    class SmoothStream : public AbstractStream
    {
        public:
            SmoothStream(demux_t *);

        protected:
            virtual AbstractDemuxer * newDemux(vlc_object_t *, const StreamFormat &,
                                               es_out_t *, AbstractSourceStream *) const override;
    };

    class SmoothStreamFactory : public AbstractStreamFactory
    {
        public:
            virtual AbstractStream *create(demux_t*, const StreamFormat &,
                                   SegmentTracker *) const override;
    };

}

#endif // SMOOTHSTREAM_HPP
