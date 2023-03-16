// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * MemoryChunk.hpp
 *****************************************************************************
 Copyright (C) 2015 - VideoLAN Authors
 *****************************************************************************/
#ifndef MEMORYCHUNK_HPP
#define MEMORYCHUNK_HPP

#include "../../adaptive/http/Chunk.h"

namespace smooth
{
    namespace http
    {
        using namespace adaptive::http;

        class MemoryChunkSource : public AbstractChunkSource
        {
            public:
                MemoryChunkSource(ChunkType, block_t *);
                virtual ~MemoryChunkSource();

                virtual block_t * readBlock() override;
                virtual block_t * read(size_t) override;
                virtual bool      hasMoreData() const override;
                virtual size_t    getBytesRead() const  override;
                virtual void      recycle() override;

            private:
                block_t *data;
                size_t   i_read;
        };

    }
}

#endif // MEMORYCHUNK_HPP
