// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * media_utils.h - media utils function
 */

/**********************************************************************
 *  Copyright (C) 2021 VLC authors and VideoLAN                       *
 **********************************************************************/

static void
libvlc_media_parse_finished_event(const libvlc_event_t *p_ev, void *p_data)
{
    (void) p_ev;
    vlc_sem_t *p_sem = p_data;
    vlc_sem_post(p_sem);
}

static inline void
libvlc_media_parse_sync(libvlc_instance_t *vlc, libvlc_media_t *p_m,
                        libvlc_media_parse_flag_t parse_flag, int timeout)
{
    vlc_sem_t sem;
    vlc_sem_init(&sem, 0);

    libvlc_event_manager_t *p_em = libvlc_media_event_manager(p_m);
    libvlc_event_attach(p_em, libvlc_MediaParsedChanged,
                        libvlc_media_parse_finished_event, &sem);

    int i_ret = libvlc_media_parse_request(vlc, p_m, parse_flag, timeout);
    assert(i_ret == 0);

    vlc_sem_wait (&sem);

    libvlc_event_detach(p_em, libvlc_MediaParsedChanged,
                        libvlc_media_parse_finished_event, &sem);
}
