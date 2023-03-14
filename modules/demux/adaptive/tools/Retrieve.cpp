/*
 * Parser.cpp
 *****************************************************************************
 * Copyright © 2015 - VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "Retrieve.hpp"

#include "../http/HTTPConnectionManager.h"
#include "../http/HTTPConnection.hpp"
#include "../http/Chunk.h"
#include "../SharedResources.hpp"

#include <vlc_block.h>

using namespace adaptive;
using namespace adaptive::http;

block_t * Retrieve::HTTP(SharedResources *resources, ChunkType type,
                         const std::string &uri)
{
    HTTPChunk *datachunk;
    try
    {
        datachunk = new HTTPChunk(uri, resources->getConnManager(),
                                  ID(), type, BytesRange());
    } catch (...) {
        return nullptr;
    }

    block_t *p_head = nullptr;
    block_t **pp_tail = &p_head;
    for(;;)
    {
        block_t *p_block = datachunk->readBlock();
        if(!p_block)
            break;
        block_ChainLastAppend(&pp_tail, p_block);
    }
    delete datachunk;

    return p_head ? block_ChainGather(p_head) : nullptr;
}
