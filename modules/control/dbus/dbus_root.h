// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * dbus_root.h : dbus control module (mpris v2.2) - Root object
 *****************************************************************************
 * Copyright © 2006-2008 Rafaël Carré
 * Copyright © 2007-2010 Mirsal Ennaime
 * Copyright © 2009-2010 The VideoLAN team
 *
 * Authors:    Mirsal Ennaime <mirsal at mirsal fr>
 *             Rafaël Carré <funman at videolanorg>
 *****************************************************************************/

#ifndef VLC_DBUS_DBUS_ROOT_H_
#define VLC_DBUS_DBUS_ROOT_H_

#include "dbus_common.h"

/* DBUS IDENTIFIERS */
#define DBUS_MPRIS_ROOT_INTERFACE    "org.mpris.MediaPlayer2"

/* Handle incoming dbus messages */
DBusHandlerResult handle_root ( DBusConnection *p_conn,
                                DBusMessage *p_from,
                                void *p_this );
int RootPropertiesChangedEmit ( intf_thread_t *,
                                vlc_dictionary_t * );

#endif /* include-guard */
