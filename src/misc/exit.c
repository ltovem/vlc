/*****************************************************************************
 * exit.c: LibVLC termination event
 *****************************************************************************
 * Copyright (C) 2009-2010 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_interface.h>
#include "libvlc.h"
#include "../lib/libvlc_internal.h"

void vlc_ExitInit( vlc_exit_t *exit )
{
    vlc_mutex_init( &exit->lock );
    exit->handler = NULL;
    exit->opaque = NULL;
}

/**
 * Registers a callback for the LibVLC exit event.
 */
void libvlc_SetExitHandler( libvlc_int_t *p_libvlc, void (*handler) (void *),
                            void *opaque )
{
    vlc_exit_t *exit = &libvlc_priv( p_libvlc )->exit;

    vlc_mutex_lock( &exit->lock );
    exit->handler = handler;
    exit->opaque = opaque;
    vlc_mutex_unlock( &exit->lock );
}

/**
 * Posts an exit signal to LibVLC instance.
 * This function should only be called on behalf of the user.
 */
void libvlc_Quit( libvlc_int_t *p_libvlc )
{
    vlc_exit_t *exit = &libvlc_priv( p_libvlc )->exit;

    msg_Dbg( p_libvlc, "exiting" );
    vlc_mutex_lock( &exit->lock );
    if( exit->handler != NULL )
        exit->handler( exit->opaque );
    else
        msg_Dbg( p_libvlc, "no exit handler" );
    vlc_mutex_unlock( &exit->lock );
}
