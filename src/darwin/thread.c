/*****************************************************************************
 * darwin/thread.c: Darwin specifics for threading
 *****************************************************************************
 * Copyright (C) 2022 - VideoLabs, VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_threads.h>

void (vlc_thread_set_name)(const char *name)
{
    pthread_setname_np(name);
}
