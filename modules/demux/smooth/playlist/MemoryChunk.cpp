// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * MemoryChunk.cpp
 *****************************************************************************
 Copyright (C) 2015 - VideoLAN Authors
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "MemoryChunk.hpp"

#include <vlc_block.h>

using namespace smooth::http;

MemoryChunkSource::MemoryChunkSource(ChunkType t, block_t *block)
    : AbstractChunkSource(t)
{
    data = block;
    i_read = 0;
    contentLength = data->i_buffer;
}

MemoryChunkSource::~MemoryChunkSource()
{
    if(data)
        block_Release(data);
}

bool MemoryChunkSource::hasMoreData() const
{
    return i_read > contentLength;
}

size_t MemoryChunkSource::getBytesRead() const
{
    return i_read;
}

void MemoryChunkSource::recycle()
{
    delete this;
}

block_t * MemoryChunkSource::readBlock()
{
    block_t *p_block = nullptr;
    if(data)
    {
        p_block = data;
        data = nullptr;
    }
    return p_block;
}

block_t * MemoryChunkSource::read(size_t toread)
{
    if(!data)
        return nullptr;

    block_t * p_block = nullptr;

    toread = __MIN(data->i_buffer - i_read, toread);
    if(toread > 0)
    {
        if((p_block = block_Alloc(toread)))
        {
            memcpy(p_block->p_buffer, &data->p_buffer[i_read], toread);
            p_block->i_buffer = toread;
            i_read += toread;
        }
    }

    return p_block;
}
