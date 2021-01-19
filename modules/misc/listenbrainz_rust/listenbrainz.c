/*****************************************************************************
 * listenbrainz.c : listenbrainz submission plugin
 *****************************************************************************
 * Copyright © 2021 Johan 'Enulp' Pardo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_interface.h>
#include "listenbrainz_rust.h"

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/

static int  Open            (vlc_object_t *);
static void Close           (vlc_object_t *);

/*****************************************************************************
 * Module descriptor
 ****************************************************************************/

#define URL_TEXT       N_("Url")
#define URL_LONGTEXT   N_("The url of your/lastbrainz server")
#define PASSWORD_TEXT       N_("Password")
#define PASSWORD_LONGTEXT   N_("The token of your lastbrainz account")

/* last.fm client identifier */
#define CLIENT_NAME     PACKAGE
#define CLIENT_VERSION  VERSION

vlc_module_begin ()
    set_category(CAT_INTERFACE)
    set_subcategory(SUBCAT_INTERFACE_CONTROL)
    set_shortname(N_("Listenbrainz"))
    set_description(N_("Submission of played songs to listenbrainz"))
    add_string("listenbrainz-url", "",
            URL_TEXT, URL_LONGTEXT, false)

    add_password("listenbrainz-token", "", PASSWORD_TEXT, PASSWORD_LONGTEXT)
    set_capability("interface", 0)
    set_callbacks(Open, Close)
vlc_module_end ()

/*****************************************************************************
 * Open: initialize and create stuff
 *****************************************************************************/
static int Open(vlc_object_t *p_this)
{
    intf_thread_t   *p_intf     = (intf_thread_t*) p_this;
    void *sys = listenbrainz_Open(p_intf);
    if (sys == NULL)
        return VLC_EGENERIC;
    p_intf->p_sys = sys;
    return VLC_SUCCESS;
}

static void Close(vlc_object_t *p_this){}

