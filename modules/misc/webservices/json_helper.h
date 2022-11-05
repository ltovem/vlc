/*****************************************************************************
 * json_helper.h:
 *****************************************************************************
 * Copyright (C) 2012-2019 VLC authors, VideoLabs and VideoLAN
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
#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <vlc_common.h>
#include <vlc_stream.h>

#include <limits.h>

#include "../../demux/json/json.h"

static inline
struct json_value json_getbyname(const struct json_object *object, const char *psz_name)
{
    struct json_member *members = object->members;
    unsigned int i = 0;
    for (i = 0; i < object->count; i++) {
        if (strcmp(members[i].name, psz_name) == 0)
            break ;
    }
    return object->members[i].value;
}

static inline
char * jsongetstring(const struct json_value *node, const char *key)
{
    const struct json_value *value = json_get(&node->object, key);

    if (!value)
        return NULL;
    if (value->type != JSON_STRING)
        return NULL;
    return value->string;
}

static inline
char * json_dupstring(const struct json_value *node, const char *key)
{
    const char *str = jsongetstring(node, key);
    return (str) ? strdup(str) : NULL;
}

static inline
void * json_retrieve_document(vlc_object_t *p_obj, const char *psz_url)
{
    bool saved_no_interact = p_obj->no_interact;
    p_obj->no_interact = true;
    stream_t *p_stream = vlc_stream_NewURL(p_obj, psz_url);

    p_obj->no_interact = saved_no_interact;
    if (p_stream == NULL)
        return NULL;

    stream_t *p_chain = vlc_stream_FilterNew(p_stream, "inflate");
    if(p_chain)
        p_stream = p_chain;

    /* read answer */
    char *p_buffer = NULL;
    int i_ret = 0;
    for(;;)
    {
        int i_read = 65536;

        if(i_ret >= INT_MAX - i_read)
            break;

        p_buffer = realloc_or_free(p_buffer, 1 + i_ret + i_read);
        if(unlikely(p_buffer == NULL))
        {
            vlc_stream_Delete(p_stream);
            return NULL;
        }

        i_read = vlc_stream_Read(p_stream, &p_buffer[i_ret], i_read);
        if(i_read <= 0)
            break;

        i_ret += i_read;
    }
    vlc_stream_Delete(p_stream);
    p_buffer[i_ret] = 0;

    return p_buffer;
}

static inline
size_t Read(void *data, void *buf, size_t size)
{
    struct sys_json *sys = (struct sys_json *)data;
    size_t ret = 0;

    if (size <= sys->size) {
        memcpy(buf, sys->input, size);
        ret = size;
    }
    else {
        memcpy(buf, sys->input, sys->size);
        ret = sys->size;
    }
    sys->input += ret;
    sys->size -= ret;

    return ret;
}

#endif
