/*****************************************************************************
 * playlist/export.c
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_playlist_export.h>

#include <errno.h>
#include <stdio.h>

#include <vlc_common.h>
#include <vlc_fs.h>
#include <vlc_modules.h>
#include <vlc_url.h>
#include "playlist.h"
#include "libvlc.h"

struct vlc_playlist_view
{
    vlc_playlist_t *playlist;
};

size_t
vlc_playlist_view_Count(struct vlc_playlist_view *view)
{
    return vlc_playlist_Count(view->playlist);
}

vlc_playlist_item_t *
vlc_playlist_view_Get(struct vlc_playlist_view *view, size_t index)
{
    return vlc_playlist_Get(view->playlist, index);
}

int
vlc_playlist_Export(struct vlc_playlist *playlist, const char *filename,
                    const char *type)
{
    vlc_playlist_AssertLocked(playlist);

    struct vlc_playlist_export *export =
        vlc_custom_create(vlc_player_GetObject(playlist->player),
                          sizeof(*export), "playlist export");

    if (!export)
        return VLC_ENOMEM;

    int ret = VLC_EGENERIC;

    struct vlc_playlist_view playlist_view = { .playlist = playlist };

    export->playlist_view = &playlist_view;
    export->base_url = vlc_path2uri(filename, NULL);
    export->file = vlc_fopen(filename, "wt");
    if (!export->file)
    {
        msg_Err(export, "Could not create playlist file %s, %s",
                filename, vlc_strerror_c(errno));
        goto close_file;
    }

    // this will actually export
    module_t *module = module_need(export, "playlist export", type, true);

    if (!module)
    {
        msg_Err(export, "Could not export playlist");
        goto out;
    }

    module_unneed(export, module);

    if (!ferror(export->file))
        ret = VLC_SUCCESS;
    else
        msg_Err(export, "Could not write playlist file: %s",
                vlc_strerror_c(errno));

close_file:
    fclose(export->file);
out:
   free(export->base_url);
   vlc_object_delete(export);
   return ret;
}
