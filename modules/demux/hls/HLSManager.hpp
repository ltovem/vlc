/*
 * HLSManager.hpp
 *****************************************************************************
 * Copyright © 2015 - VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef HLSMANAGER_HPP
#define HLSMANAGER_HPP

#include "../adaptive/PlaylistManager.h"
#include "../adaptive/logic/AbstractAdaptationLogic.h"
#include "playlist/M3U8.hpp"

namespace hls
{
    using namespace adaptive;

    class HLSManager : public PlaylistManager
    {
        public:
            HLSManager( demux_t *,
                        SharedResources *,
                        playlist::M3U8 *,
                        AbstractStreamFactory *,
                        logic::AbstractAdaptationLogic::LogicType type );
            virtual ~HLSManager();
            static bool isHTTPLiveStreaming(stream_t *);
    };

}

#endif // HLSMANAGER_HPP
