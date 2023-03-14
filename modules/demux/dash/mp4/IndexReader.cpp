/*
 * IndexReader.cpp
 *****************************************************************************
 * Copyright (C) 2015 - VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "IndexReader.hpp"
#include "../mpd/Representation.h"
#include "../mpd/MPD.h"

using namespace adaptive::mp4;
using namespace dash::mp4;
using namespace dash::mpd;

IndexReader::IndexReader(vlc_object_t *obj)
    : AtomsReader(obj)
{
}

bool IndexReader::parseIndex(block_t *p_block, BaseRepresentation *rep, uint64_t i_fileoffset)
{
    if(!rep || !parseBlock(p_block))
        return false;

    MP4_Box_t *sidxbox = MP4_BoxGet(rootbox, "sidx");
    if (sidxbox)
    {
        Representation::SplitPoint point;
        std::vector<Representation::SplitPoint> splitlist;
        MP4_Box_data_sidx_t *sidx = sidxbox->data.p_sidx;
        /* sidx refers to offsets from end of sidx pos in the file + first offset */
        point.offset = sidx->i_first_offset + i_fileoffset + sidxbox->i_pos + sidxbox->i_size;
        point.time = 0;
        if(!sidx->i_timescale)
            return false;
        for(uint16_t i=0; i<sidx->i_reference_count; i++)
        {
            splitlist.push_back(point);
            point.offset += sidx->p_items[i].i_referenced_size;
            point.duration = sidx->p_items[i].i_subsegment_duration;
            point.time += point.duration;
        }
        rep->replaceAttribute(new TimescaleAttr(Timescale(sidx->i_timescale)));
        rep->SplitUsingIndex(splitlist);
        rep->getPlaylist()->debug();
    }

    return true;
}
