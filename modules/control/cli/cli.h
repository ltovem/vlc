// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * rc.c : remote control stdin/stdout module for vlc
 *****************************************************************************
 * Copyright (C) 2004-2009 the VideoLAN team
 *
 * Author: Peter Surda <shurdeek@panorama.sth.ac.at>
 *         Jean-Paul Saman <jpsaman #_at_# m2x _replaceWith#dot_ nl>
 *****************************************************************************/

#include <stdatomic.h>
#include <stdio.h>
#include <vlc_common.h>
#include <vlc_list.h>

struct cli_client
{
    intf_thread_t *intf;
#ifndef _WIN32
    FILE *stream;
    int fd;
    atomic_bool zombie;
    vlc_mutex_t output_lock;
    struct vlc_list node;
    vlc_thread_t thread;
#endif
};

VLC_FORMAT(2, 3)
int cli_printf(struct cli_client *cl, const char *fmt, ...);

VLC_FORMAT(2, 3)
void msg_print(intf_thread_t *p_intf, const char *psz_fmt, ...);

#define msg_rc(...) cli_printf(cl, __VA_ARGS__)
#define STATUS_CHANGE "status change: "

typedef int (*cli_callback)(struct cli_client *, const char *const *, size_t,
                            void *);

struct cli_handler
{
    const char *name;
    cli_callback callback;
};

void RegisterHandlers(intf_thread_t *intf, const struct cli_handler *handlers,
                      size_t count, void *opaque);

void *RegisterPlayer(intf_thread_t *intf);
void DeregisterPlayer(intf_thread_t *intf, void *);

void RegisterPlaylist(intf_thread_t *intf);
