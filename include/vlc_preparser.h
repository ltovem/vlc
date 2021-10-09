/*****************************************************************************
 * vlc_preparser.h : Preparsing API
 *****************************************************************************
 * Copyright (C) 2021 Videolabs, VLC authors and VideoLAN
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

#ifndef VLC_PREPARSER_H
#define VLC_PREPARSER_H

#include <vlc_common.h>
#include <vlc_input_item.h>

struct vlc_preparser_task
{
    input_item_t* item;
    input_item_meta_request_option_t options;
    const input_preparser_callbacks_t* cbs;
    void* data;
    int timeout;
    void* id;
};

struct vlc_preparser
{
    struct vlc_object_t obj;

    module_t *module;
    void* sys;

    int (*pf_preparse)(struct vlc_preparser*, struct vlc_preparser_task*);

    void (*pf_cancel)(struct vlc_preparser*, void*);
};

struct vlc_preparser* vlc_preparser_New(vlc_object_t*);
void vlc_preparser_Delete(struct vlc_preparser*);

/**
 * This function enqueues the provided item to be preparsed.
 *
 * The input item is retained until the preparsing is done or until the
 * preparser object is deleted.
 *
 * @param timeout maximum time allowed to preparse the item. If -1, the default
 * "preparse-timeout" option will be used as a timeout. If 0, it will wait
 * indefinitely. If > 0, the timeout will be used (in milliseconds).
 * @param id unique id provided by the caller. This is can be used to cancel
 * the request with input_preparser_Cancel()
 * @returns VLC_SUCCESS if the item was scheduled for preparsing, an error code
 * otherwise
 * If this returns an error, the on_preparse_ended will *not* be invoked
 */
int vlc_preparser_Preparse(struct vlc_preparser*, input_item_t*,
                           input_item_meta_request_option_t,
                           const input_preparser_callbacks_t*,
                           void*, int, void*);

void vlc_preparser_Cancel(struct vlc_preparser*, void*);

void vlc_preparser_fetcher_Push( struct vlc_preparser *preparser,
    input_item_t *item, input_item_meta_request_option_t options,
    const input_fetcher_callbacks_t *cbs, void *cbs_userdata );

#endif // VLC_PREPARSER_H
