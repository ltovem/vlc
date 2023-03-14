/*
 * HLSSegment.cpp
 *****************************************************************************
 * Copyright (C) 2015 VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "HLSSegment.hpp"
#include "../../adaptive/playlist/BaseRepresentation.h"


using namespace hls::playlist;

HLSSegment::HLSSegment( ICanonicalUrl *parent, uint64_t seq ) :
    Segment( parent )
{
    setSequenceNumber(seq);
}

HLSSegment::~HLSSegment()
{
}

bool HLSSegment::prepareChunk(SharedResources *res, SegmentChunk *chunk, BaseRepresentation *rep)
{
    if(encryption.method == CommonEncryption::Method::AES_128)
    {
        if (encryption.iv.size() != 16)
        {
            uint64_t sequence = getSequenceNumber();
            encryption.iv.clear();
            encryption.iv.resize(16);
            encryption.iv[15] = (sequence >> 0) & 0xff;
            encryption.iv[14] = (sequence >> 8) & 0xff;
            encryption.iv[13] = (sequence >> 16) & 0xff;
            encryption.iv[12] = (sequence >> 24) & 0xff;
        }
    }

    return Segment::prepareChunk(res, chunk, rep);
}
