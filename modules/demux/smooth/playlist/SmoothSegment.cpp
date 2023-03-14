/*****************************************************************************
 * SmoothSegment.cpp:
 *****************************************************************************
 * Copyright (C) 2015 - VideoLAN Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "SmoothSegment.hpp"

#include "../../adaptive/playlist/BaseRepresentation.h"
#include "../../adaptive/playlist/BasePlaylist.hpp"
#include "../mp4/SmoothIndexReader.hpp"

using namespace smooth::playlist;
using namespace smooth::mp4;

SmoothSegmentChunk::SmoothSegmentChunk(AbstractChunkSource *source, BaseRepresentation *rep)
    : SegmentChunk(source, rep)
{

}

SmoothSegmentChunk::~SmoothSegmentChunk()
{

}

void SmoothSegmentChunk::onDownload(block_t **pp_block)
{
    decrypt(pp_block);

    if(!rep || ((*pp_block)->i_flags & BLOCK_FLAG_HEADER) == 0)
        return;

    SmoothIndexReader br(rep->getPlaylist()->getVLCObject());
    br.parseIndex(*pp_block, rep, sequence);

    /* If timeshift depth is present, we use it for expiring segments
       as we never update playlist itself */
    if(rep->getPlaylist()->timeShiftBufferDepth.Get())
    {
        vlc_tick_t start, end, length;
        if(rep->getMediaPlaybackRange(&start, &end, &length))
        {
            start = std::max(start, end - rep->getPlaylist()->timeShiftBufferDepth.Get());
            rep->pruneByPlaybackTime(start);
        }
    }
}

SmoothSegmentTemplateSegment::SmoothSegmentTemplateSegment(ICanonicalUrl *parent)
    : SegmentTemplateSegment(parent)
{

}

SmoothSegmentTemplateSegment::~SmoothSegmentTemplateSegment()
{

}

SegmentChunk* SmoothSegmentTemplateSegment::createChunk(AbstractChunkSource *source, BaseRepresentation *rep)
{
     /* act as factory */
    return new (std::nothrow) SmoothSegmentChunk(source, rep);
}

