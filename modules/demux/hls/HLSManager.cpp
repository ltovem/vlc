/*****************************************************************************
 * HLSManager.cpp
 *****************************************************************************
 * Copyright © 2015 VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "HLSManager.hpp"
#include "../adaptive/tools/Retrieve.hpp"
#include "../adaptive/http/HTTPConnectionManager.h"
#include "playlist/Parser.hpp"
#include <vlc_stream.h>
#include <vlc_demux.h>
#include <time.h>

using namespace adaptive;
using namespace adaptive::logic;
using namespace hls;
using namespace hls::playlist;

HLSManager::HLSManager(demux_t *demux_,
                       SharedResources *res,
                       M3U8 *playlist,
                       AbstractStreamFactory *factory,
                       AbstractAdaptationLogic::LogicType type) :
             PlaylistManager(demux_, res, playlist, factory, type)
{
}

HLSManager::~HLSManager()
{
}

bool HLSManager::isHTTPLiveStreaming(stream_t *s)
{
    const uint8_t *peek;

    int size = vlc_stream_Peek(s, &peek, 7);
    if (size < 7 || memcmp(peek, "#EXTM3U", 7))
        return false;

    size = vlc_stream_Peek(s, &peek, 8192);
    if (size < 7)
        return false;

    peek += 7;
    size -= 7;

    /* Parse stream and search for
     * EXT-X-TARGETDURATION or EXT-X-STREAM-INF tag, see
     * http://tools.ietf.org/html/draft-pantos-http-live-streaming-04#page-8 */
    while (size--)
    {
        static const char *const ext[] = {
            "TARGETDURATION",
            "MEDIA-SEQUENCE",
            "KEY",
            "ALLOW-CACHE",
            "ENDLIST",
            "STREAM-INF",
            "DISCONTINUITY",
            "VERSION"
        };

        if (*peek++ != '#')
            continue;

        if (size < 6)
            continue;

        if (memcmp(peek, "EXT-X-", 6))
            continue;

        peek += 6;
        size -= 6;

        for (size_t i = 0; i < ARRAY_SIZE(ext); i++)
        {
            size_t len = strlen(ext[i]);
            if (size < 0 || (size_t)size < len)
                continue;
            if (!memcmp(peek, ext[i], len))
                return true;
        }
    }

    return false;
}
