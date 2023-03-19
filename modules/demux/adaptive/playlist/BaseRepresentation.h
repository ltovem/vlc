// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Representation.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef BASEREPRESENTATION_H_
#define BASEREPRESENTATION_H_

#include <string>
#include <list>

#include "CommonAttributesElements.h"
#include "CodecDescription.hpp"
#include "SegmentInformation.hpp"
#include "../StreamFormat.hpp"

namespace adaptive
{
    class SharedResources;

    namespace playlist
    {
        class BaseAdaptationSet;
        class BasePlaylist;
        class SegmentTemplateSegment;

        class BaseRepresentation : public CommonAttributesElements,
                                   public SegmentInformation
        {
            public:
                BaseRepresentation( BaseAdaptationSet * );
                virtual ~BaseRepresentation ();

                virtual StreamFormat getStreamFormat() const;
                BaseAdaptationSet* getAdaptationSet();
                /*
                 *  @return The bitrate required for this representation
                 *          in bits per seconds.
                 *          Will be a valid value, as the parser refuses Representation
                 *          without bandwidth.
                 */
                uint64_t            getBandwidth            () const;
                void                setBandwidth            ( uint64_t bandwidth );
                const std::list<std::string> & getCodecs    () const;
                void                addCodecs               (const std::string &);
                void                getCodecsDesc           (CodecDescriptionList *) const;
                virtual void        pruneByPlaybackTime     (vlc_tick_t) override;

                virtual vlc_tick_t  getMinAheadTime         (uint64_t) const;
                virtual bool        needsUpdate             (uint64_t) const;
                virtual bool        needsIndex              () const;
                virtual bool        runLocalUpdates         (SharedResources *);
                virtual void        scheduleNextUpdate      (uint64_t, bool);
                virtual bool        canNoLongerUpdate       () const;

                virtual void        debug                   (vlc_object_t *,int = 0) const;

                /* for segment templates */
                virtual std::string contextualize(size_t, const std::string &,
                                                  const SegmentTemplate *) const;

                static bool         bwCompare(const BaseRepresentation *a,
                                              const BaseRepresentation *b);

                virtual uint64_t translateSegmentNumber(uint64_t, const BaseRepresentation *) const;
                bool getSegmentNumberByTime(vlc_tick_t, uint64_t *) const;
                bool getPlaybackTimeDurationBySegmentNumber(uint64_t, vlc_tick_t *, vlc_tick_t *) const;
                bool getMediaPlaybackRange(vlc_tick_t *rangeBegin,
                                                               vlc_tick_t *rangeEnd,
                                                               vlc_tick_t *rangeLength) const;
            protected:
                virtual CodecDescription * makeCodecDescription(const std::string &) const;
                virtual bool        validateCodec(const std::string &) const;
                BaseAdaptationSet                  *adaptationSet;
                uint64_t                            bandwidth;
                std::list<std::string>              codecs;
        };
    }
}

#endif /* BASEREPRESENTATION_H_ */
