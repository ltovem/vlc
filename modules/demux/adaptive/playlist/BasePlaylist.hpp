// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * BasePlaylist.hpp
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University
 Copyright (C) 2015 - 2020 VideoLabs, VideoLAN and VLC authors
 *****************************************************************************/
#ifndef BASEPLAYLIST_HPP_
#define BASEPLAYLIST_HPP_

#include <vector>
#include <string>

#include "ICanonicalUrl.hpp"
#include "Inheritables.hpp"
#include "../tools/Properties.hpp"

namespace adaptive
{

    namespace playlist
    {
        class BasePeriod;

        class BasePlaylist : public ICanonicalUrl,
                             public AttrsNode
        {
            public:
                BasePlaylist(vlc_object_t *);
                virtual ~BasePlaylist();

                virtual bool                    isLive() const;
                virtual bool                    isLowLatency() const;
                void                            setType(const std::string &);
                void                            setMinBuffering( vlc_tick_t );
                void                            setMaxBuffering( vlc_tick_t );
                vlc_tick_t                      getMinBuffering() const;
                vlc_tick_t                      getMaxBuffering() const;
                virtual void                    debug() const;

                void    addPeriod               (BasePeriod *period);
                void    addBaseUrl              (const std::string &);
                void    setPlaylistUrl          (const std::string &);

                virtual Url         getUrlSegment() const override;
                vlc_object_t *      getVLCObject()  const;

                virtual const std::vector<BasePeriod *>& getPeriods();
                virtual BasePeriod*                      getFirstPeriod();
                virtual BasePeriod*                      getNextPeriod(BasePeriod *period);

                bool                needsUpdates() const;
                void                updateWith(BasePlaylist *);

                Property<vlc_tick_t>                   duration;
                Property<time_t>                    playbackStart;
                Property<vlc_tick_t>                   availabilityEndTime;
                Property<vlc_tick_t>                   availabilityStartTime;
                Property<vlc_tick_t>                   minUpdatePeriod;
                Property<vlc_tick_t>                   maxSegmentDuration;
                Property<vlc_tick_t>                   timeShiftBufferDepth;
                Property<vlc_tick_t>                   suggestedPresentationDelay;
                Property<vlc_tick_t>                   presentationStartOffset;

            protected:
                vlc_object_t                       *p_object;
                std::vector<BasePeriod *>           periods;
                std::vector<std::string>            baseUrls;
                std::string                         playlistUrl;
                std::string                         type;
                vlc_tick_t                          minBufferTime;
                vlc_tick_t                          maxBufferTime;
                bool                                b_needsUpdates;
        };
    }
}
#endif /* BASEPLAYLIST_HPP_ */
