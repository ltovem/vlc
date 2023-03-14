/*****************************************************************************
 * dummy.c
 *****************************************************************************
 * Copyright (C) 2001, 2002, 2004 VLC authors and VideoLAN
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_sout.h>
#include <vlc_block.h>

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int  Open ( vlc_object_t * );

vlc_module_begin ()
    set_description( N_("Dummy stream output") )
    set_shortname( N_( "Dummy" ))
    set_capability( "sout access", 0 )
    set_subcategory( SUBCAT_SOUT_ACO )
    add_shortcut( "dummy" )
    set_callback( Open )
vlc_module_end ()


/*****************************************************************************
 * Exported prototypes
 *****************************************************************************/
static ssize_t Write( sout_access_out_t *, block_t * );

/*****************************************************************************
 * Open: open the file
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    sout_access_out_t   *p_access = (sout_access_out_t*)p_this;

    p_access->pf_write = Write;

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Read: standard read on a file descriptor.
 *****************************************************************************/
static ssize_t Write( sout_access_out_t *p_access, block_t *p_buffer )
{
    size_t i_write = 0;
    block_t *b = p_buffer;

    while( b )
    {
        i_write += b->i_buffer;

        b = b->p_next;
    }

    block_ChainRelease( p_buffer );

    (void)p_access;
    return i_write;
}
