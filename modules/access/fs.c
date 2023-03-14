/*****************************************************************************
 * fs.c: file system access plugin
 *****************************************************************************
 * Copyright (C) 2001-2006 VLC authors and VideoLAN
 * Copyright © 2006-2007 Rémi Denis-Courmont
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *          Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include "fs.h"
#include <vlc_plugin.h>

vlc_module_begin ()
    set_description( N_("File input") )
    set_shortname( N_("File") )
    set_subcategory( SUBCAT_INPUT_ACCESS )
    set_capability( "access", 50 )
    add_shortcut( "file", "fd", "stream" )
    set_callbacks( FileOpen, FileClose )

    add_submodule()
    set_section( N_("Directory" ), NULL )
    set_capability( "access", 55 )
#ifndef HAVE_FDOPENDIR
    add_shortcut( "file", "directory", "dir" )
#else
    add_shortcut( "directory", "dir" )
#endif
    set_callbacks( DirOpen, DirClose )

    add_bool("list-special-files", false, N_("List special files"),
             N_("Include devices and pipes when listing directories"))
    add_obsolete_string("directory-sort") /* since 3.0.0 */
vlc_module_end ()
