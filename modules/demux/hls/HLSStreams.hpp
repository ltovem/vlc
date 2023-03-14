/*
 * HLSStreams.hpp
 *****************************************************************************
 * Copyright (C) 2015 - VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef HLSSTREAM_HPP
#define HLSSTREAM_HPP

#include "../adaptive/Streams.hpp"

namespace hls
{
    using namespace adaptive;

    class HLSStream : public AbstractStream
    {
        public:
            HLSStream(demux_t *);
            virtual ~HLSStream();

        protected:
            virtual block_t *checkBlock(block_t *, bool) override;
            virtual AbstractDemuxer * newDemux(vlc_object_t *, const StreamFormat &,
                                               es_out_t *, AbstractSourceStream *) const override;
            virtual bool setPosition(const StreamPosition &, bool) override;
            virtual void trackerEvent(const TrackerEvent &)  override;

        private:
            static int ID3TAG_Parse_Handler(uint32_t, const uint8_t *, size_t, void *);
            int ParseID3Tag(uint32_t, const uint8_t *, size_t);
            int ParseID3PrivTag(const uint8_t *, size_t);
            void setMetadataTimeOffset(vlc_tick_t);
            void setMetadataTimeMapping(vlc_tick_t, vlc_tick_t);
            bool b_id3_timestamps_offset_set;
            vlc_meta_t *p_meta;
            bool b_meta_updated;
    };

    class HLSStreamFactory : public AbstractStreamFactory
    {
        public:
            virtual AbstractStream *create(demux_t *, const StreamFormat &,
                                   SegmentTracker *) const override;
    };

}
#endif // HLSSTREAMS_HPP
