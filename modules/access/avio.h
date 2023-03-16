// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * avio.h: access using libavformat library
 *****************************************************************************
 * Copyright (C) 2009 Laurent Aimar
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *****************************************************************************/

#include <libavformat/avformat.h>
#include <libavformat/avio.h>

int  OpenAvio (vlc_object_t *);
void CloseAvio(vlc_object_t *);
int  OutOpenAvio (vlc_object_t *);
void OutCloseAvio(vlc_object_t *);

#define AVIO_MODULE \
    set_shortname(N_("AVIO"))                                                    \
    set_description(N_("libavformat AVIO access") )                              \
    set_subcategory(SUBCAT_INPUT_ACCESS)                                         \
    set_capability("access", -1)                                                 \
    add_shortcut("avio", "rtmp", "rtmpe", "rtmps", "rtmpt", "rtmpte", "rtmpts")  \
    set_callbacks(OpenAvio, CloseAvio)                                           \
    set_section(N_("Input"), NULL )                                              \
    add_string("avio-options", NULL, AV_OPTIONS_TEXT, AV_OPTIONS_LONGTEXT) \
    add_submodule ()                                                             \
        set_shortname( "AVIO" )                                                  \
        set_description( N_("libavformat AVIO access output") )                  \
        set_capability( "sout access", -1 )                                      \
        set_subcategory( SUBCAT_SOUT_ACO )                                       \
        add_shortcut( "avio", "rtmp" )                                           \
        set_callbacks( OutOpenAvio, OutCloseAvio)                                \
        set_section(N_("Stream output"), NULL )                                  \
        add_string("sout-avio-options", NULL, AV_OPTIONS_TEXT, AV_OPTIONS_LONGTEXT)
