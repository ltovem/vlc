/*****************************************************************************
 * darwin/thread.c: Darwin specifics for threading
 *****************************************************************************
 * Copyright (C) 2022 - VideoLabs, VLC authors and VideoLAN
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

#include <vlc_common.h>
#include <vlc_threads.h>

#include <assert.h>
#include <pthread.h>

#include "../posix/thread.h"

void (vlc_thread_set_name)(const char *name)
{
    pthread_setname_np(name);
}

struct named_thread
{
    const char *name;
    void *(*entry) (void *);
    void *data;
};

static void * VlcNamedThread(void *opaque)
{
    struct named_thread *init = opaque;
    vlc_thread_set_name(init->name);
    void *(*entry) (void *) = init->entry;
    void *data = init->data;
    free(init);
    return entry(data);
}

int vlc_clone_attr (vlc_thread_t *th, pthread_attr_t *attr,
                    void *(*entry) (void *), void *data, const char *name)
{
    int ret;

    /* The thread stack size.
     * The lower the value, the less address space per thread, the highest
     * maximum simultaneous threads per process. Too low values will cause
     * stack overflows and weird crashes. Set with caution. Also keep in mind
     * that 64-bits platforms consume more stack than 32-bits one.
     *
     * Thanks to on-demand paging, thread stack size only affects address space
     * consumption. In terms of memory, threads only use what they need
     * (rounded up to the page boundary).
     *
     * For example, on Linux i386, the default is 2 mega-bytes, which supports
     * about 320 threads per processes. */
#define VLC_STACKSIZE (128 * sizeof (void *) * 1024)

#ifdef VLC_STACKSIZE
    ret = pthread_attr_setstacksize (attr, VLC_STACKSIZE);
    assert (ret == 0); /* fails iff VLC_STACKSIZE is invalid */
#endif

    if (name == NULL)
       ret = pthread_create(&th->handle, attr, entry, data);
    else
    {
        struct named_thread *init = malloc(sizeof(*init));
        if (unlikely(init == NULL))
            return -1;
        init->name = name;
        init->entry = entry;
        init->data = data;
        ret = pthread_create(&th->handle, attr, VlcNamedThread, init);
    }
    return ret;
}