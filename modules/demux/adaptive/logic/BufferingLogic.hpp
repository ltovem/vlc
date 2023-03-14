/*
 * BufferingLogic.hpp
 *****************************************************************************
 * Copyright (C) 2014 - 2020 VideoLabs, VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef BUFFERINGLOGIC_HPP
#define BUFFERINGLOGIC_HPP

#include <vlc_common.h>

#include "../tools/Properties.hpp"

namespace adaptive
{
    namespace playlist
    {
        class BaseRepresentation;
        class BasePlaylist;
    }

    namespace logic
    {
        using namespace playlist;

        class AbstractBufferingLogic
        {
            public:
                AbstractBufferingLogic();
                virtual ~AbstractBufferingLogic() {}

                virtual uint64_t getStartSegmentNumber(BaseRepresentation *) const = 0;
                virtual vlc_tick_t getMinBuffering(const BasePlaylist *) const = 0;
                virtual vlc_tick_t getMaxBuffering(const BasePlaylist *) const = 0;
                virtual vlc_tick_t getLiveDelay(const BasePlaylist *) const = 0;
                virtual vlc_tick_t getStableBuffering(const BasePlaylist *) const = 0;
                void setUserMinBuffering(vlc_tick_t);
                void setUserMaxBuffering(vlc_tick_t);
                void setUserLiveDelay(vlc_tick_t);
                void setLowDelay(bool);
                static const vlc_tick_t BUFFERING_LOWEST_LIMIT;
                static const vlc_tick_t DEFAULT_MIN_BUFFERING;
                static const vlc_tick_t DEFAULT_MAX_BUFFERING;
                static const vlc_tick_t DEFAULT_LIVE_BUFFERING;

            protected:
                vlc_tick_t userMinBuffering;
                vlc_tick_t userMaxBuffering;
                vlc_tick_t userLiveDelay;
                Undef<bool> userLowLatency;
        };

        class DefaultBufferingLogic : public AbstractBufferingLogic
        {
            public:
                DefaultBufferingLogic();
                virtual ~DefaultBufferingLogic() {}
                virtual uint64_t getStartSegmentNumber(BaseRepresentation *) const override;
                virtual vlc_tick_t getMinBuffering(const BasePlaylist *) const override;
                virtual vlc_tick_t getMaxBuffering(const BasePlaylist *) const override;
                virtual vlc_tick_t getLiveDelay(const BasePlaylist *) const override;
                virtual vlc_tick_t getStableBuffering(const BasePlaylist *) const override;
                static const unsigned SAFETY_BUFFERING_EDGE_OFFSET;
                static const unsigned SAFETY_EXPURGING_OFFSET;

            protected:
                vlc_tick_t getBufferingOffset(const BasePlaylist *) const;
                uint64_t getLiveStartSegmentNumber(BaseRepresentation *) const;
                bool isLowLatency(const BasePlaylist *) const;
        };
    }
}

#endif
