/**
 * \file vlcpulse.h
 * \brief PulseAudio support library for LibVLC plugins
 */
/****************************************************************************
 * Copyright (C) 2011 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLCPULSE_H
# define VLCPULSE_H 1
# ifdef __cplusplus
extern "C" {
# endif

VLC_API pa_context *vlc_pa_connect (vlc_object_t *obj,
                                    pa_threaded_mainloop **);
VLC_API void vlc_pa_disconnect (vlc_object_t *obj, pa_context *ctx,
                                pa_threaded_mainloop *);

VLC_API void vlc_pa_error (vlc_object_t *, const char *msg, pa_context *);
#define vlc_pa_error(o, m, c) vlc_pa_error(VLC_OBJECT(o), m, c)

VLC_API vlc_tick_t vlc_pa_get_latency (vlc_object_t *, pa_context *, pa_stream *);
#define vlc_pa_get_latency(o, c, s) vlc_pa_get_latency(VLC_OBJECT(o), c, s)

VLC_API void vlc_pa_rttime_free (pa_threaded_mainloop *, pa_time_event *);

# ifdef __cplusplus
}
# endif
#endif
