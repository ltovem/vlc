/*****************************************************************************
 * guess_type.h: Helper to probe the access for the type of an input_item
 *****************************************************************************
 * Copyright © 2022 VLC authors and VideoLAN
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

#ifndef GUESS_TYPE_H
#define GUESS_TYPE_H

#include <vlc_common.h>
#include <vlc_modules.h>
#include <vlc_stream.h>
#include <vlc_input_item.h>

static inline int GuessTypeRun(void* func, bool forced, va_list va)
{
    VLC_UNUSED(forced);
    vlc_guesstype_activate_cb cb = (vlc_guesstype_activate_cb)func;
    enum input_item_type_e* type_ptr = va_arg(va, enum input_item_type_e*);
    bool* net_ptr = va_arg(va, bool*);
    const struct vlc_stream_guessType_Entry* entry = cb();
    if( !entry )
        return VLC_EGENERIC;
    *type_ptr = entry->type;
    *net_ptr = entry->net;
    return VLC_SUCCESS;
}

/**
 * @brief input_item_GuessType Probes the access module for the item type
 * @param obj A vlc_object_t*
 * @param item The input item for which to probe the type
 */
static inline void input_item_GuessType(vlc_object_t* obj, input_item_t* item)
{
    const char* scheme_end = strchr(item->psz_uri, ':');
    char* scheme = strndup(item->psz_uri, scheme_end - item->psz_uri);
    if( scheme )
    {
        module_t* m = vlc_module_load(obj, "guesstype", scheme, true,
                                      GuessTypeRun, &item->i_type, &item->b_net);
        free(scheme);
        if( m )
            module_unneed(obj, m);
    }
}

#endif // GUESS_TYPE_H
