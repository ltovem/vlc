/*****************************************************************************
 * vlc_meta_fetcher.h
 *****************************************************************************
 * Copyright (C) 2009 Rémi Denis-Courmont
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

#ifndef VLC_META_FETCHER_H
#define VLC_META_FETCHER_H 1

#include <vlc_input.h>

typedef enum meta_fetcher_scope_t
{
    FETCHER_SCOPE_LOCAL   = 0x01,
    FETCHER_SCOPE_NETWORK = 0x02,
    FETCHER_SCOPE_ANY     = 0x03
} meta_fetcher_scope_t;

typedef struct meta_fetcher_t
{
    struct vlc_object_t obj;
    input_item_t *p_item;
    meta_fetcher_scope_t e_scope;
    input_attachment_t *attachment_artwork;
} meta_fetcher_t;

static inline int
meta_fetcher_SetAttachmentArtwork(meta_fetcher_t *fetcher, const char *mime,
                                  const void *data, size_t data_size)
{
    fetcher->attachment_artwork =
        vlc_input_attachment_New("meta_fetcher_artwork", mime, NULL, data,
                                 data_size);
    if (fetcher->attachment_artwork == NULL)
        return VLC_ENOMEM;

    input_item_SetArtworkURL(fetcher->p_item, "attachment://meta_fetcher_artwork");

    return VLC_SUCCESS;
}

#endif
