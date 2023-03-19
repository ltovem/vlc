// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * DBMSDIOutput.hpp: Decklink SDI Output
 *****************************************************************************
 * Copyright © 2014-2016 VideoLAN and VideoLAN Authors
 *                  2018 VideoLabs
 *****************************************************************************/
#ifndef DBMSDIOUTPUT_HPP
#define DBMSDIOUTPUT_HPP

#include "SDIOutput.hpp"

#include <vlc_es.h>
#include "../../access/vlc_decklink.h"

namespace sdi_sout
{
    class DBMSDIOutput : public SDIOutput,
                         public IDeckLinkVideoOutputCallback
    {
        public:
            DBMSDIOutput(sout_stream_t *);
            ~DBMSDIOutput();
            virtual AbstractStream *Add(const es_format_t *); /* reimpl */
            virtual int Open(); /* impl */
            virtual int Process(); /* impl */

            virtual HRESULT STDMETHODCALLTYPE QueryInterface (REFIID, LPVOID *);
            virtual ULONG STDMETHODCALLTYPE AddRef ();
            virtual ULONG STDMETHODCALLTYPE Release ();
            virtual HRESULT STDMETHODCALLTYPE ScheduledFrameCompleted (IDeckLinkVideoFrame *, BMDOutputFrameCompletionResult);
            virtual HRESULT STDMETHODCALLTYPE ScheduledPlaybackHasStopped (void);

        protected:
            int ProcessVideo(picture_t *, block_t *);
            int ProcessAudio(block_t *);
            virtual int ConfigureVideo(const video_format_t *); /* impl */
            virtual int ConfigureAudio(const audio_format_t *); /* impl */

        private:
            IDeckLink *p_card;
            IDeckLinkOutput *p_output;

            BMDTimeScale timescale;
            BMDTimeValue frameduration;
            vlc_tick_t lasttimestamp;
            /* XXX: workaround card clock drift */
            struct
            {
                vlc_tick_t offset; /* > 0 if clock card is slower */
                vlc_tick_t system_reference;
                BMDTimeValue hardware_reference;
            } clock;
            bool b_running;
            vlc_tick_t streamStartTime;
            int StartPlayback();
            struct
            {
                vlc_mutex_t lock; /* Driver calls callback... until buffer is empty :/ */
                vlc_cond_t cond;
                vlc_thread_t thread;
                bool canceled;
            } feeder;
            static void *feederThreadCallback(void *);
            void feederThread();
            int doSchedule();
            int doProcessVideo(picture_t *, block_t *);
            int FeedOneFrame();
            int FeedAudio(vlc_tick_t, vlc_tick_t, bool);
            void checkClockDrift();
            bool isDrained();
    };
}

#endif // DBMSDIOUTPUT_HPP
