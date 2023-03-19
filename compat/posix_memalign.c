// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * posix_memalign.c: POSIX posix_memalign() replacement
 *****************************************************************************
 * Copyright © 2012, 2019 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <stdlib.h>
#include <limits.h>

#ifdef HAVE_MEMALIGN
# include <malloc.h>
#else

static void *memalign(size_t align, size_t size)
{
    void *p = malloc(size);

    if ((uintptr_t)p & (align - 1)) {
        free(p);
        p = NULL;
    }

    return p;
}

#endif

static int check_align(size_t align)
{
    if (align & (align - 1)) /* must be a power of two */
        return EINVAL;
    if (align < sizeof (void *)) /* must be a multiple of sizeof (void *) */
        return EINVAL;
    return 0;
}

int posix_memalign(void **ptr, size_t align, size_t size)
{
    int val = check_align(align);
    if (val)
        return val;

    /* Unlike posix_memalign(), legacy memalign() requires that size be a
     * multiple of align.
     */
    if (size > (SIZE_MAX / 2))
        return ENOMEM;

    size += (-size) & (align - 1);

    int saved_errno = errno;
    void *p = memalign(align, size);
    if (p == NULL) {
        val = errno;
        errno = saved_errno;
        return val;
    }

    *ptr = p;
    return 0;
}
