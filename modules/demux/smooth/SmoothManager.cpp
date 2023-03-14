/*
 * SmoothManager.cpp
 *****************************************************************************
 * Copyright © 2015 - VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cinttypes>

#include "SmoothManager.hpp"

#include "../adaptive/SharedResources.hpp"
#include "../adaptive/tools/Retrieve.hpp"
#include "playlist/SmoothParser.hpp"
#include "../adaptive/xml/DOMParser.h"
#include <vlc_stream.h>
#include <vlc_demux.h>
#include <vlc_charset.h>
#include <time.h>

using namespace adaptive;
using namespace adaptive::logic;
using namespace smooth;
using namespace smooth::playlist;

SmoothManager::SmoothManager(demux_t *demux_,
                             SharedResources *res,
                             Manifest *playlist,
                       AbstractStreamFactory *factory,
                       AbstractAdaptationLogic::LogicType type) :
             PlaylistManager(demux_, res, playlist, factory, type)
{
}

SmoothManager::~SmoothManager()
{
}

Manifest * SmoothManager::fetchManifest()
{
    std::string playlisturl(p_demux->psz_url);

    block_t *p_block = Retrieve::HTTP(resources, ChunkType::Playlist, playlisturl);
    if(!p_block)
        return nullptr;

    stream_t *memorystream = vlc_stream_MemoryNew(p_demux, p_block->p_buffer, p_block->i_buffer, true);
    if(!memorystream)
    {
        block_Release(p_block);
        return nullptr;
    }

    xml::DOMParser parser(memorystream);
    if(!parser.parse(true))
    {
        vlc_stream_Delete(memorystream);
        block_Release(p_block);
        return nullptr;
    }

    Manifest *manifest = nullptr;

    ManifestParser *manifestParser = new (std::nothrow) ManifestParser(parser.getRootNode(), VLC_OBJECT(p_demux),
                                                                       memorystream, playlisturl);
    if(manifestParser)
    {
        manifest = manifestParser->parse();
        delete manifestParser;
    }

    vlc_stream_Delete(memorystream);
    block_Release(p_block);

    return manifest;
}

bool SmoothManager::updatePlaylist()
{
    bool b_playlist_empty = false;
    /* Trigger full playlist update in case we cannot get next
       segment from atom */
    std::vector<AbstractStream *>::const_iterator it;
    for(it=streams.begin(); it!=streams.end(); ++it)
    {
        const AbstractStream *st = *it;
        const vlc_tick_t m = st->getMinAheadTime();
        if(!st->isValid() || st->isDisabled() || !st->isSelected())
        {
            continue;
        }
        else if(m < 1)
        {
            b_playlist_empty = true;
            break;
        }
    }
    return updatePlaylist(b_playlist_empty);
}

void SmoothManager::scheduleNextUpdate()
{
    time_t now = time(nullptr);

    vlc_tick_t minbuffer = getMinAheadTime() / 2;

    if(playlist->minUpdatePeriod.Get() > minbuffer)
        minbuffer = playlist->minUpdatePeriod.Get();

    minbuffer = std::max(minbuffer, VLC_TICK_FROM_SEC(5));

    nextPlaylistupdate = now + SEC_FROM_VLC_TICK(minbuffer);

    msg_Dbg(p_demux, "Updated playlist, next update in %" PRId64 "s", (int64_t) nextPlaylistupdate - now );
}

bool SmoothManager::needsUpdate() const
{
    if(nextPlaylistupdate && time(nullptr) < nextPlaylistupdate)
        return false;

    return PlaylistManager::needsUpdate();
}

bool SmoothManager::updatePlaylist(bool forcemanifest)
{
    /* FIXME: do update from manifest after resuming from pause */

    /* Timelines updates should be inlined in tfrf atoms.
       We'll just care about pruning live timeline then. */

    if(forcemanifest && nextPlaylistupdate)
    {
        Manifest *newManifest = fetchManifest();
        if(newManifest)
        {
            playlist->updateWith(newManifest);
            delete newManifest;

#ifdef NDEBUG
            playlist->debug();
#endif
        }
        else return false;
    }

    return true;
}

bool SmoothManager::reactivateStream(AbstractStream *stream)
{
    if(playlist->isLive())
        updatePlaylist(true);
    return PlaylistManager::reactivateStream(stream);
}

bool SmoothManager::isSmoothStreaming(xml::Node *root)
{
    return root->getName() == "SmoothStreamingMedia";
}

bool SmoothManager::mimeMatched(const std::string &mime)
{
    return (mime == "application/vnd.ms-sstr+xml");
}
