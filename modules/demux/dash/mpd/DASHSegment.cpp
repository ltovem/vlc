// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Segment.cpp
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "DASHSegment.h"
#include "../../adaptive/playlist/BaseRepresentation.h"
#include "../mp4/IndexReader.hpp"
#include "../../adaptive/playlist/BasePlaylist.hpp"

using namespace adaptive::playlist;
using namespace dash::mpd;
using namespace dash::mp4;

DashIndexChunk::DashIndexChunk(AbstractChunkSource *source, BaseRepresentation *rep)
    : SegmentChunk(source, rep)
{

}

DashIndexChunk::~DashIndexChunk()
{

}

void DashIndexChunk::onDownload(block_t **pp_block)
{
    decrypt(pp_block);

    if(!rep || ((*pp_block)->i_flags & BLOCK_FLAG_HEADER) == 0 )
        return;

    IndexReader br(rep->getPlaylist()->getVLCObject());
    br.parseIndex(*pp_block, rep, getStartByteInFile());
}

DashIndexSegment::DashIndexSegment(ICanonicalUrl *parent) :
    IndexSegment(parent)
{
}

DashIndexSegment::~DashIndexSegment()
{

}

SegmentChunk* DashIndexSegment::createChunk(AbstractChunkSource *source, BaseRepresentation *rep)
{
     /* act as factory */
    return new (std::nothrow) DashIndexChunk(source, rep);
}
