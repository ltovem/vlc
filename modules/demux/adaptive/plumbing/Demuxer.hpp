// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Demuxer.hpp
 *****************************************************************************
 Copyright © 2015 - VideoLAN and VLC Authors
 *****************************************************************************/
#ifndef DEMUXER_HPP
#define DEMUXER_HPP

#include <vlc_common.h>
#include <string>

namespace adaptive
{
    class AbstractSourceStream;
    class DemuxerFactoryInterface;
    class StreamFormat;

    class AbstractDemuxer
    {
        public:
            enum class Status
            {
                Success,
                Error,
                Eof,
            };
            AbstractDemuxer();
            virtual ~AbstractDemuxer();
            virtual Status demux(vlc_tick_t) = 0;
            virtual void drain() = 0;
            virtual bool create() = 0;
            virtual void destroy() = 0;
            bool alwaysStartsFromZero() const;
            bool needsRestartOnSeek() const;
            bool bitstreamSwitchCompatible() const;
            bool needsRestartOnEachSegment() const;
            void setBitstreamSwitchCompatible(bool);
            void setRestartsOnEachSegment(bool);

        protected:
            static Status returnCode(int);
            bool b_startsfromzero;
            bool b_reinitsonseek;
            bool b_alwaysrestarts;
            bool b_candetectswitches;
    };

    class Demuxer : public AbstractDemuxer
    {
        public:
            Demuxer(vlc_object_t *, const std::string &, es_out_t *, AbstractSourceStream *);
            virtual ~Demuxer();
            virtual Status demux(vlc_tick_t) override;
            virtual void drain() override;
            virtual bool create() override;
            virtual void destroy() override;

        protected:
            AbstractSourceStream *sourcestream;
            vlc_object_t *p_obj;
            demux_t *p_demux;
            std::string name;
            es_out_t *p_es_out;
            bool b_eof;
    };

    class SlaveDemuxer : public Demuxer
    {
        public:
            SlaveDemuxer(vlc_object_t *, const std::string &, es_out_t *, AbstractSourceStream *);
            virtual ~SlaveDemuxer();
            virtual bool create() override;
            virtual Status demux(vlc_tick_t) override;

        private:
            vlc_tick_t length;
    };

    class DemuxerFactoryInterface
    {
        public:
            virtual AbstractDemuxer * newDemux(vlc_object_t *, const StreamFormat &,
                                               es_out_t *, AbstractSourceStream *) const = 0;
            virtual ~DemuxerFactoryInterface() = default;
    };
}

#endif // DEMUXER_HPP
