/*****************************************************************************
 * common.h
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#include <vlc/vlc.h>

#if 0
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...) (void)0
#endif

struct vlc_run_args
{
    /* force specific target name (demux or decoder name). NULL to don't force
     * any */
    const char *name;

    /* vlc verbose level */
    unsigned verbose;

    /* true to test demux controls */
    bool test_demux_controls;
};

void vlc_run_args_init(struct vlc_run_args *args);

libvlc_instance_t *libvlc_create(const struct vlc_run_args *args);
