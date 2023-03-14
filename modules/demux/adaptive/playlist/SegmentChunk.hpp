/*
 * SegmentChunk.hpp
 *****************************************************************************
 * Copyright (C) 2014 - 2015 VideoLAN Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef SEGMENTCHUNK_HPP
#define SEGMENTCHUNK_HPP

#include "../http/Chunk.h"
#include "../StreamFormat.hpp"

namespace adaptive
{
    namespace encryption
    {
        class CommonEncryptionSession;
    }

    namespace playlist
    {
        using namespace http;
        using namespace encryption;

        class BaseRepresentation;

        class SegmentChunk : public AbstractChunk
        {
        public:
            SegmentChunk(AbstractChunkSource *, BaseRepresentation *);
            virtual ~SegmentChunk();
            void         setEncryptionSession(CommonEncryptionSession *);
            StreamFormat getStreamFormat() const;
            void setStreamFormat(const StreamFormat &);
            bool discontinuity;
            uint64_t discontinuitySequenceNumber;
            uint64_t sequence;

        protected:
            bool         decrypt(block_t **);
            virtual void onDownload(block_t **) override;
            BaseRepresentation *rep;
            StreamFormat format;
            CommonEncryptionSession *encryptionSession;
        };

    }
}
#endif // SEGMENTCHUNK_HPP
