/*****************************************************************************
 * dbus_tracklist.h : dbus control module (mpris v2.2) - TrackList object
 *****************************************************************************
 * Copyright © 2006-2008 Rafaël Carré
 * Copyright © 2007-2010 Mirsal Ennaime
 * Copyright © 2009-2010 The VideoLAN team
 *
 * Authors:    Mirsal Ennaime <mirsal at mirsal fr>
 *             Rafaël Carré <funman at videolanorg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VLC_DBUS_DBUS_TRACKLIST_H_
#define VLC_DBUS_DBUS_TRACKLIST_H_

#include <vlc_common.h>
#include <vlc_interface.h>
#include "dbus_common.h"

#define DBUS_MPRIS_TRACKLIST_INTERFACE    "org.mpris.MediaPlayer2.TrackList"
#define DBUS_MPRIS_TRACKLIST_PATH         "/org/mpris/MediaPlayer2/TrackList"

#define DBUS_MPRIS_NOTRACK   "/org/mpris/MediaPlayer2/TrackList/NoTrack"
#define DBUS_MPRIS_APPEND    "/org/mpris/MediaPlayer2/TrackList/Append"

struct tracklist_change_event {
    size_t index;
    size_t count;
    struct tracklist_change_event *next;
};

struct tracklist_append_event {
    struct tracklist_change_event change_ev;
    vlc_playlist_item_t *items[];
};

struct tracklist_remove_event {
    struct tracklist_change_event change_ev;
};

typedef struct tracklist_append_event tracklist_append_event_t;
typedef struct tracklist_remove_event tracklist_remove_event_t;

/* Creates an event holding what items have been appended to a tracklist.
 * The event data will be used to generate TrackAdded DBus signals later on.
 */
tracklist_append_event_t *
tracklist_append_event_create( size_t index,
                               vlc_playlist_item_t *const items[],
                               size_t count );

/* Creates an event holding what items have been removed from a tracklist.
 *  The event data will be used to generate TrackRemoved DBus signals later on.
 */
tracklist_remove_event_t *
tracklist_remove_event_create( size_t index, size_t count );

/* Releases any resources reserved for this event */
void tracklist_append_event_destroy( tracklist_append_event_t *event );
void tracklist_remove_event_destroy( tracklist_remove_event_t *event );

/* Gets next event in the list */
static tracklist_append_event_t *
tracklist_append_event_next( tracklist_append_event_t *event ) {
    if( !event )
        return NULL;
    char *p = (char *) event->change_ev.next;
    return (tracklist_append_event_t *)
        (p - offsetof(struct tracklist_append_event, change_ev));
}

static tracklist_remove_event_t *
tracklist_remove_event_next( tracklist_remove_event_t *event ) {
    if( !event )
        return NULL;
    char *p = (char *) event->change_ev.next;
    return (tracklist_remove_event_t *)
        (p - offsetof(struct tracklist_remove_event, change_ev));
}

/* Handle incoming dbus messages */
DBusHandlerResult handle_tracklist ( DBusConnection *p_conn,
                                     DBusMessage *p_from,
                                     void *p_this );

int TrackListPropertiesChangedEmit( intf_thread_t *, vlc_dictionary_t * );

#endif /* include-guard */
