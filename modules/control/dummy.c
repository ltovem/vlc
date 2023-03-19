// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * intf_dummy.c: dummy interface plugin
 *****************************************************************************
 * Copyright (C) 2000, 2001 the VideoLAN team
 *
 * Authors: Samuel Hocevar <sam@zoy.org>
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#define VLC_MODULE_LICENSE VLC_LICENSE_GPL_2_PLUS
#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_interface.h>

static int Open( vlc_object_t * );

vlc_module_begin ()
    set_shortname( N_("Dummy") )
    set_description( N_("Dummy interface") )
    set_capability( "interface", 0 )
    set_callback( Open )
#if defined(_WIN32) && !defined(VLC_WINSTORE_APP)
    add_obsolete_bool( "dummy-quiet" ) /* since 4.0.0 */
#endif
vlc_module_end ()

/*****************************************************************************
 * Open: initialize dummy interface
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    intf_thread_t *p_intf = (intf_thread_t*) p_this;

    msg_Info( p_intf, "using the dummy interface module..." );

    return VLC_SUCCESS;
}
