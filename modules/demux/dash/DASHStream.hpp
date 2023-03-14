/*
 * DASHStream.hpp
 *****************************************************************************
 * Copyright (C) 2015 - VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef DASHSTREAM_HPP
#define DASHSTREAM_HPP

#include "../adaptive/Streams.hpp"

namespace dash
{
    using namespace adaptive;

    class DASHStream : public AbstractStream
    {
        public:
            DASHStream(demux_t *);

        protected:
            virtual AbstractDemuxer * newDemux(vlc_object_t *, const StreamFormat &,
                                               es_out_t *, AbstractSourceStream *) const override;
    };

    class DASHStreamFactory : public AbstractStreamFactory
    {
        public:
            virtual AbstractStream *create(demux_t*, const StreamFormat &,
                                   SegmentTracker *) const override;
    };
}

#endif // DASHSTREAM_HPP
