/*****************************************************************************
 * inflate_rust.c: zlib decompression module for VLC
 *****************************************************************************
 * Copyright © 2020 Johan 'Enulp' Pardo
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_stream.h>
#include "src/vlccore_sys/vlccore_sys.h"
#include "inflate_rust/inflate_rust.h"

static ssize_t Read(stream_t *stream, void *buf, size_t buflen)
{
    return vlcrs_stream_filter_read(stream, buf, buflen);
}

static int Seek(stream_t *stream, uint64_t offset)
{
    (void) stream; (void) offset;
    return -1;
}

static int Control(stream_t *stream, int query, va_list args)
{
    switch (query)
    {
        case STREAM_CAN_SEEK:
        case STREAM_CAN_FASTSEEK:
            *va_arg(args, bool *) = false;
            break;
        case STREAM_CAN_PAUSE:
        case STREAM_CAN_CONTROL_PACE:
        case STREAM_GET_PTS_DELAY:
        case STREAM_GET_META:
        case STREAM_GET_CONTENT_TYPE:
        case STREAM_GET_SIGNAL:
        case STREAM_SET_PAUSE_STATE:
            return vlc_stream_vaControl(stream->s, query, args);
        case STREAM_GET_SIZE:
        case STREAM_GET_TITLE_INFO:
        case STREAM_GET_TITLE:
        case STREAM_GET_SEEKPOINT:
        case STREAM_SET_TITLE:
        case STREAM_SET_SEEKPOINT:
        case STREAM_SET_PRIVATE_ID_STATE:
        case STREAM_SET_PRIVATE_ID_CA:
        case STREAM_GET_PRIVATE_ID_STATE:
            return VLC_EGENERIC;
        default:
            msg_Err(stream, "unimplemented query (%d) in control", query);
            return VLC_EGENERIC;
    }
    return VLC_SUCCESS;
}

static int Open(vlc_object_t *obj)
{
    stream_t *stream = (stream_t *)obj;
    const uint8_t *peek;
    int bits;

    /* See IETF RFC6713 */
    if (vlc_stream_Peek(stream->s, &peek, 2) < 2)
        return VLC_EGENERIC;

    if ((peek[0] & 0xF) == 8 && (peek[0] >> 4) < 8 && (U16_AT(peek) % 31) == 0)
        bits = 15; /* zlib */
    else
    if (!memcmp(peek, "\x1F\x8B", 2))
        bits = 15 + 32; /* gzip */
    else
        return VLC_EGENERIC;

    void *cinflate = inflate_new(stream->s, bits);
    if (cinflate == NULL)
    {
        vlc_stream_Seek(stream->s, 0);
        return VLC_EGENERIC;
    }
    stream->p_sys = cinflate;
    stream->pf_read = Read;
    stream->pf_seek = Seek;
    stream->pf_control = Control;
    return VLC_SUCCESS;
}

static void Close (vlc_object_t *obj)
{
    stream_t *stream = (stream_t *)obj;
    vlcrs_stream_filter_close(stream);
}

vlc_module_begin()
    set_category(CAT_INPUT)
    set_subcategory(SUBCAT_INPUT_STREAM_FILTER)
    set_capability("stream_filter", 330)

    set_description(N_("Zlib decompression filter"))
    set_callbacks(Open, Close)
vlc_module_end()
