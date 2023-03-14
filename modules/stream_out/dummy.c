/*****************************************************************************
 * dummy.c: dummy stream output module
 *****************************************************************************
 * Copyright (C) 2003-2004 VLC authors and VideoLAN
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
#include <vlc_block.h>
#include <vlc_sout.h>

static void *Add( sout_stream_t *p_stream, const es_format_t *p_fmt )
{
    VLC_UNUSED(p_stream); VLC_UNUSED(p_fmt);
    return malloc( 1 );
}

static void Del( sout_stream_t *p_stream, void *id )
{
    VLC_UNUSED(p_stream);
    free( id );
}

static int Send( sout_stream_t *p_stream, void *id, block_t *p_buffer )
{
    (void)p_stream; (void)id;
    block_ChainRelease( p_buffer );
    return VLC_SUCCESS;
}

static const struct sout_stream_operations ops = {
    Add, Del, Send, NULL, NULL, NULL,
};

/*****************************************************************************
 * Open:
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    sout_stream_t *p_stream = (sout_stream_t*)p_this;

    p_stream->ops = &ops;
    return VLC_SUCCESS;
}

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
vlc_module_begin()
    set_description(N_("Dummy stream output"))
    set_capability("sout output", 50)
    add_shortcut("dummy", "drop")
    set_callback(Open )
vlc_module_end()
