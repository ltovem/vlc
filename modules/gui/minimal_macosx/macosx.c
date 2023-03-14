/*****************************************************************************
 * macosx.c: minimal Mac OS X module for vlc
 *****************************************************************************
 * Copyright (C) 2001-2012 VLC authors and VideoLAN
 *
 * Authors: Colin Delacroix <colin@zoy.org>
 *          Eugenio Jarosiewicz <ej0@cise.ufl.edu>
 *          Pierre d'Herbemont <pdherbemont # videolan.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include <stdlib.h>                                      /* malloc(), free() */
#include <string.h>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_window.h>

/*****************************************************************************
 * External prototypes
 *****************************************************************************/
int  OpenIntf     ( vlc_object_t * );
void CloseIntf    ( vlc_object_t * );

int  WindowOpen   ( vlc_window_t * );

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/

vlc_module_begin ()
    /* Minimal interface. see intf.m */
    set_shortname( "Minimal Macosx" )
    add_shortcut( "minimal_macosx", "miosx" )
    set_description( N_("Minimal Mac OS X interface") )
    set_capability( "interface", 50 )
    set_callbacks( OpenIntf, CloseIntf )
    set_subcategory( SUBCAT_INTERFACE_MAIN )

    add_submodule ()
    /* Will be loaded even without interface module. see voutgl.m */
        set_description( "Minimal Mac OS X Video Output Provider" )
        set_capability( "vout window", 50 )
        set_callback( WindowOpen )
vlc_module_end ()

