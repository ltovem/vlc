// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * posix/thread.h: define local POSIX core thread functions
 *****************************************************************************
 * Copyright © 2023 Videolabs, VLC authors and VideoLAN
 *
 * Authors: Steve Lhomme <robux4@videolabs.io>
 *****************************************************************************/

#pragma once

#if defined(__linux__) || defined(__FreeBSD__)
# define VLC_SET_EXT_THREAD_NAME 1
#endif

void posix_thread_set_name(pthread_t, const char *);

int vlc_clone_attr(vlc_thread_t *th,
                   void *(*entry) (void *), void *data, const char *name);
