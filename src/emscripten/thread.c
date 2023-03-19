// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * vlc_thread implementation for emscripten
 *****************************************************************************
 * Copyright (C) 2021 - VideoLabs, VideoLAN and VLC Authors
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_threads.h>
#include <pthread.h>
#include <assert.h>

unsigned long vlc_thread_id(void)
{
    static_assert(sizeof(pthread_t) <= sizeof(unsigned long),"invalid pthread_t size");
    return (uintptr_t)(void *)pthread_self();
}
