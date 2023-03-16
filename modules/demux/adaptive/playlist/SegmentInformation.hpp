// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * SegmentInformation.hpp
 *****************************************************************************
 Copyright (C) 2014 - VideoLAN and VLC Authors
 *****************************************************************************/
#ifndef SEGMENTINFORMATION_HPP
#define SEGMENTINFORMATION_HPP

#include "ICanonicalUrl.hpp"
#include "Inheritables.hpp"
#include "Segment.h"
#include "../tools/Properties.hpp"
#include "../encryption/CommonEncryption.hpp"
#include <vlc_common.h>
#include <vector>

namespace adaptive
{
    namespace playlist
    {
        class AbstractSegmentBaseType;
        class SegmentBase;
        class SegmentList;
        class SegmentTimeline;
        class SegmentTemplate;
        class SegmentTemplate;
        class BasePlaylist;
        class ISegment;

        /* common segment elements for period/adaptset/rep 5.3.9.1,
         * with properties inheritance */
        class SegmentInformation : public ICanonicalUrl,
                                   public Unique,
                                   public AttrsNode
        {
            friend class AbstractMultipleSegmentBaseType;

            public:
                SegmentInformation( SegmentInformation * = 0 );
                explicit SegmentInformation( BasePlaylist * );
                virtual ~SegmentInformation();

                virtual vlc_tick_t getPeriodStart() const;
                virtual vlc_tick_t getPeriodDuration() const;
                virtual BasePlaylist *getPlaylist() const;

                class SplitPoint
                {
                    public:
                        size_t offset;
                        stime_t time;
                        stime_t duration;
                };
                void SplitUsingIndex(std::vector<SplitPoint>&);

                virtual InitSegment * getInitSegment() const;
                virtual IndexSegment *getIndexSegment() const;
                virtual Segment *     getMediaSegment(uint64_t = 0) const;
                virtual Segment *     getNextMediaSegment(uint64_t, uint64_t *, bool *) const;

                virtual void updateWith(SegmentInformation *);
                virtual void pruneBySegmentNumber(uint64_t);
                virtual void pruneByPlaybackTime(vlc_tick_t);
                void setEncryption(const CommonEncryption &);
                const CommonEncryption & intheritEncryption() const;

            protected:
                std::vector<SegmentInformation *> childs;
                SegmentInformation * getChildByID( const ID & );
                SegmentInformation *parent;

            public:
                AbstractSegmentBaseType *getProfile() const;
                void updateSegmentList(SegmentList *, bool = false);
                void setSegmentTemplate(SegmentTemplate *);
                virtual Url getUrlSegment() const override;
                Property<Url *> baseUrl;
                const AbstractSegmentBaseType * inheritSegmentProfile() const;

            private:
                void init();
                CommonEncryption commonEncryption;
        };
    }
}

#endif // SEGMENTINFORMATION_HPP
