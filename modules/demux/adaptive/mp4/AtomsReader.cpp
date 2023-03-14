/*
 * AtomsReader.cpp
 *****************************************************************************
 * Copyright (C) 2014 - VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "AtomsReader.hpp"
#include <new>

using namespace adaptive::mp4;

AtomsReader::AtomsReader(vlc_object_t *object_)
{
    object = object_;
    rootbox = nullptr;
}

AtomsReader::~AtomsReader()
{
    clean();
}

void AtomsReader::clean()
{
    MP4_BoxFree(rootbox);
    rootbox = nullptr;
}

bool AtomsReader::parseBlock(block_t *p_block)
{
    if(rootbox)
        clean();

    stream_t *stream = vlc_stream_MemoryNew( object, p_block->p_buffer, p_block->i_buffer, true);
    if (stream)
    {
        rootbox = MP4_BoxNew(ATOM_root);
        if(!rootbox)
        {
            vlc_stream_Delete(stream);
            return false;
        }
        memset(rootbox, 0, sizeof(*rootbox));
        rootbox->i_type = ATOM_root;
        rootbox->i_size = p_block->i_buffer;
        if ( MP4_ReadBoxContainerChildren( stream, rootbox, nullptr ) == 1 )
        {
#ifndef NDEBUG
            MP4_BoxDumpStructure(stream, rootbox);
#endif
        }
        vlc_stream_Delete(stream);
    }

    return true;
}
