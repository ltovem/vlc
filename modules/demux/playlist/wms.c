// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * wms.c : Windows Media Server metafile format
 *****************************************************************************
 * Copyright (C) 2004-2019 VLC authors and VideoLAN
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 * Authors: Sigmund Augdal Helberg <dnumgis@videolan.org>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_access.h>
#include <vlc_charset.h>

#include "playlist.h"

static int ReadDir(stream_t *demux, input_item_node_t *subitems)
{
    char *line;

    while ((line = vlc_stream_ReadLine(demux->s)) != NULL)
    {
        if (!IsUTF8(line))
            goto skip;
        if (!strcmp(line, "[Reference]"))
            goto skip;

        const char *key = line;
        char *value = strchr(line, '=');
        if (value == NULL) {
            msg_Warn(demux, "unexpected entry \"%s\"", line);
            goto skip;
        }
        *(value++) = '\0';

        unsigned id;
        if (sscanf(key, "Ref%u", &id) != 1) {
            msg_Warn(demux, "unexpected entry key \"%s\"", key);
            goto skip;
        }

        if (!strncasecmp(value, "http://", 7))
            memcpy(value, "mmsh", 4); /* Force MMSH access/demux */

        input_item_t *item = input_item_New(value, value);
        input_item_node_AppendItem(subitems, item);
        input_item_Release(item);
skip:
        free(line);
    }
    return VLC_SUCCESS;
}

int Import_WMS(vlc_object_t *obj)
{
    stream_t *demux = (stream_t *)obj;
    const uint8_t *peek;

    if (vlc_stream_Peek(demux->s, &peek, 10) < 10
     || strncmp((const char *)peek, "[Reference]", 11))
        return VLC_EGENERIC;

    msg_Dbg(demux, "found WMS metafile");
    demux->pf_readdir = ReadDir;
    demux->pf_control = PlaylistControl;
    return VLC_SUCCESS;
}
