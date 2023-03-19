// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * freebsd/thread.c: FreeBSD specifics for atomic wait
 *****************************************************************************
 * Copyright (C) 2020 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>
#include <stddef.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <pthread.h>
#include <pthread_np.h>
#include <sys/thr.h>
#include <sys/umtx.h>

#include <vlc_common.h>
#include <vlc_atomic.h>

unsigned long vlc_thread_id(void)
{
     static _Thread_local int tid = -1;

     if (unlikely(tid == -1))
         tid = thr_self();

     return tid;
}

void (vlc_thread_set_name)(const char *name)
{
    pthread_set_name_np(pthread_self(), name);
}

static int vlc_umtx_wake(void *addr, int nr)
{
    return _umtx_op(addr, UMTX_OP_WAKE_PRIVATE, nr, NULL, NULL);
}

static int vlc_umtx_wait(void *addr, unsigned val, const struct timespec *ts)
{
    struct _umtx_time to = {
        ._timeout = *ts,
        ._flags = UMTX_ABSTIME,
        ._clockid = CLOCK_MONOTONIC,
    };
    int type;
    /* "the uaddr value [...] must be equal to the size of the structure
     * pointed to by uaddr2, casted to uintptr_t."
     */
    void *uaddr = (void *)sizeof (to);

    return _umtx_op(addr, UMTX_OP_WAIT_UINT_PRIVATE, val, uaddr, &to);
}

void vlc_atomic_notify_one(void *addr)
{
    vlc_umtx_wake(addr, 1);
}

void vlc_atomic_notify_all(void *addr)
{
    vlc_umtx_wake(addr, INT_MAX);
}

void vlc_atomic_wait(void *addr, unsigned val)
{
    int ret = vlc_umtx_wait(addr, val, NULL);

    assert(ret == 0 || ret == EINTR || ret == ERESTART);
}

int vlc_atomic_timedwait(void *addr, unsigned val, vlc_tick_t deadline)
{
    struct timespec ts;
    int ret = vlc_umtx_wait(addr, val, vlc_tick_to_timespec(&ts, deadline));

    assert(ret == 0 || ret == ETIMEDOUT || ret == EINTR || ret == ERESTART);
    return (ret != ETIMEDOUT) ? 0 : ret;
}
