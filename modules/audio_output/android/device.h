// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * android/device.h: Android AudioTrack/AAudio device handler
 *****************************************************************************
 * Copyright © 2012-2022 VLC authors and VideoLAN, VideoLabs
 *
 * Authors: Thomas Guillem <thomas@gllm.fr>
 *****************************************************************************/

#include <jni.h>

typedef struct aout_stream aout_stream_t;

enum android_audio_device_type
{
    ANDROID_AUDIO_DEVICE_STEREO = 0,
    ANDROID_AUDIO_DEVICE_PCM,
    ANDROID_AUDIO_DEVICE_ENCODED,
};
#define ANDROID_AUDIO_DEVICE_MAX_CHANNELS 8

struct aout_stream
{
    struct vlc_object_t obj;
    void *sys;

    void (*stop)(aout_stream_t *);
    int (*time_get)(aout_stream_t *, vlc_tick_t *);
    void (*play)(aout_stream_t *, block_t *, vlc_tick_t);
    void (*pause)(aout_stream_t *, bool, vlc_tick_t);
    void (*flush)(aout_stream_t *);
    void (*drain)(aout_stream_t *);
    void (*volume_set)(aout_stream_t *, float volume);
    void (*mute_set)(aout_stream_t *, bool mute);

    audio_output_t *aout;
};

static inline void
aout_stream_GainRequest(aout_stream_t *s, float gain)
{
    aout_GainRequest(s->aout, gain);
}

static inline void
aout_stream_RestartRequest(aout_stream_t *s, unsigned mode)
{
    aout_RestartRequest(s->aout, mode);
}

static inline
void aout_stream_TimingReport(aout_stream_t *s, vlc_tick_t system_ts,
                              vlc_tick_t audio_ts)
{
    aout_TimingReport(s->aout, system_ts, audio_ts);
}

static inline
void aout_stream_DrainedReport(aout_stream_t *s)
{
    aout_DrainedReport(s->aout);
}

typedef int (*aout_stream_start)(aout_stream_t *s, audio_sample_format_t *fmt,
                                 enum android_audio_device_type dev);
