// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * picture.c:
 *****************************************************************************
 * Copyright (C) 2018 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>
#include <sys/types.h>
#include <sys/mman.h>

#include <vlc_common.h>
#include <vlc_fs.h>
#include "misc/picture.h"

void *picture_Allocate(int *restrict fdp, size_t size)
{
    int fd = vlc_memfd();
    if (fd == -1)
        return NULL;

    if (ftruncate(fd, size)) {
error:
        vlc_close(fd);
        return NULL;
    }

    void *base = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (base == MAP_FAILED)
        goto error;

    *fdp = fd;
    return base;
}

void picture_Deallocate(int fd, void *base, size_t size)
{
    munmap(base, size);
    vlc_close(fd);
}
