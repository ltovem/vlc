/*****************************************************************************
 * libvlc_internal.h : Definition of opaque structures for libvlc exported API
 * Also contains some internal utility functions
 *****************************************************************************
 * Copyright (C) 2005-2009 VLC authors and VideoLAN
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef _LIBVLC_INTERNAL_H
#define _LIBVLC_INTERNAL_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc/libvlc.h>
#include <vlc/libvlc_dialog.h>
#include <vlc/libvlc_picture.h>
#include <vlc/libvlc_media.h>
#include <vlc/libvlc_events.h>
#include <vlc_atomic.h>

#include <vlc_common.h>

/* Note well: this header is included from LibVLC core.
 * Therefore, static inline functions MUST NOT call LibVLC functions here
 * (this can cause linkage failure on some platforms). */

/***************************************************************************
 * Internal creation and destruction functions
 ***************************************************************************/
VLC_API libvlc_int_t *libvlc_InternalCreate( void );
VLC_API int libvlc_InternalInit( libvlc_int_t *, int, const char *ppsz_argv[] );
VLC_API void libvlc_InternalCleanup( libvlc_int_t * );
VLC_API void libvlc_InternalDestroy( libvlc_int_t * );

VLC_API int libvlc_InternalAddIntf( libvlc_int_t *, const char * );
VLC_API void libvlc_InternalPlay( libvlc_int_t * );
VLC_API void libvlc_InternalWait( libvlc_int_t * );
VLC_API void libvlc_SetExitHandler( libvlc_int_t *, void (*) (void *), void * );

/***************************************************************************
 * Opaque structures for libvlc API
 ***************************************************************************/

struct libvlc_instance_t
{
    libvlc_int_t *p_libvlc_int;
    vlc_atomic_rc_t ref_count;
    struct libvlc_callback_entry_list_t *p_callback_list;
    struct
    {
        void (*cb) (void *, int, const libvlc_log_t *, const char *, va_list);
        void *data;
    } log;
    struct
    {
        libvlc_dialog_cbs cbs;
        void *data;
    } dialog;
};

struct libvlc_event_manager_t
{
    void * p_obj;
    vlc_array_t listeners;
    vlc_mutex_t lock;
};

/***************************************************************************
 * Other internal functions
 ***************************************************************************/

/* Thread context */
void libvlc_threads_init (void);
void libvlc_threads_deinit (void);

/* Events */
void libvlc_event_manager_init(libvlc_event_manager_t *, void *);
void libvlc_event_manager_destroy(libvlc_event_manager_t *);

void libvlc_event_send(
        libvlc_event_manager_t * p_em,
        libvlc_event_t * p_event );

static inline libvlc_time_t libvlc_time_from_vlc_tick(vlc_tick_t time)
{
    return MS_FROM_VLC_TICK(time + VLC_TICK_FROM_US(500));
}

static inline vlc_tick_t vlc_tick_from_libvlc_time(libvlc_time_t time)
{
    return VLC_TICK_FROM_MS(time);
}

#endif
