/*****************************************************************************
 * SDIOutput.hpp: SDI sout module for vlc
 *****************************************************************************
 * Copyright © 2018 VideoLabs, VideoLAN and VideoLAN Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef SDIOUTPUT_HPP
#define SDIOUTPUT_HPP

#include "SDIStream.hpp"
#include <vlc_common.h>
#include <vlc_sout.h>
#include <list>

namespace sdi_sout
{
    class SDIAudioMultiplex;

    class SDIOutput
    {
        public:
            SDIOutput(sout_stream_t *);
            virtual ~SDIOutput();
            virtual int Open() = 0;
            virtual int Process() = 0;
            virtual AbstractStream *Add(const es_format_t *);
            virtual int   Send(AbstractStream *, block_t *);
            virtual void  Del(AbstractStream *);
            virtual int   Control(int, va_list);

        protected:
            virtual AbstractStream * createStream(const StreamID &,
                                                  const es_format_t *,
                                                  AbstractStreamOutputBuffer *,
                                                  bool = true);
            virtual int ConfigureVideo(const video_format_t *) = 0;
            virtual int ConfigureAudio(const audio_format_t *) = 0;
            virtual bool ReachedPlaybackTime(vlc_tick_t);
            sout_stream_t *p_stream;
            VideoDecodedStream *videoStream;
            std::list<AbstractStream *> audioStreams;
            CaptionsStream *captionsStream;
            PictureStreamOutputBuffer videoBuffer;
            BlockStreamOutputBuffer captionsBuffer;
            SDIAudioMultiplex *audioMultiplex;

            int program;

            struct
            {
                es_format_t configuredfmt;
                bool tenbits;
                int nosignal_delay;
                picture_t *pic_nosignal;
            } video;

            struct
            {
                uint8_t i_channels;
                bool b_configured;
            } audio;

            struct
            {
                uint8_t afd, ar;
                unsigned afd_line;
                unsigned captions_line;
            } ancillary;

        private:
            static void *SoutCallback_Add(sout_stream_t *, const es_format_t *);
            static void  SoutCallback_Del(sout_stream_t *, void *);
            static int   SoutCallback_Send(sout_stream_t *, void *, block_t*);
            static int   SoutCallback_Control(sout_stream_t *, int, va_list);
            static void  SoutCallback_Flush(sout_stream_t *, void *);
            struct sout_stream_operations ops;
    };
}

#endif
