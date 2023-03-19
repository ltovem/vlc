// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * headers.c: Test for public headers usability
 *****************************************************************************
 * Copyright (C) 2007 Rémi Denis-Courmont
 *****************************************************************************/

/* config.h is NOT installed, headers MUST NOT depend on it.
 # include <config.h> */
/* One thing we don't check is the CPPFLAGS - these MUST be exposed publicly,
 * e.g. using pkg-config. */

#define PACKAGE "vlc"

/* Because we are from src/ LIBVLC_INTERNAL_ is defined, but we don't want that,
 * as we act here as a third-party program just linking to libvlc */
#ifdef LIBVLC_INTERNAL_
# undef LIBVLC_INTERNAL_
#endif

#include <vlc/vlc.h>
#include <vlc/deprecated.h>
#include <vlc/libvlc.h>
#include <vlc/libvlc_events.h>
#include <vlc/libvlc_media.h>
#include <vlc/libvlc_media_discoverer.h>
#include <vlc/libvlc_media_list.h>
#include <vlc/libvlc_media_list_player.h>
#include <vlc/libvlc_media_player.h>

#include <stdio.h>

int main (void)
{
    puts ("Public headers can be used for external compilation.");
    return 0;
}

