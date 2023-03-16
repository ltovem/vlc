// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * aligned_alloc.c: C11 aligned_alloc() replacement
 *****************************************************************************
 * Copyright © 2012, 2017 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <assert.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#if !defined (HAVE_POSIX_MEMALIGN)
# include <malloc.h>
#endif

void *aligned_alloc(size_t align, size_t size)
{
    /* align must be a power of 2 */
    /* size must be a multiple of align */
    if ((align & (align - 1)) || (size & (align - 1)))
    {
        errno = EINVAL;
        return NULL;
    }

#ifdef HAVE_POSIX_MEMALIGN
    if (align < sizeof (void *)) /* POSIX does not allow small alignment */
        align = sizeof (void *);

    void *ptr;
    int err = posix_memalign(&ptr, align, size);
    if (err)
    {
        errno = err;
        ptr = NULL;
    }
    return ptr;

#elif defined(HAVE_MEMALIGN)
    return memalign(align, size);
#elif defined (_WIN32) && defined(__MINGW32__)
    return __mingw_aligned_malloc(size, align);
#elif defined (_WIN32) && defined(_MSC_VER)
    return _aligned_malloc(size, align);
#else
    /* align must be valid/supported */
    assert(align <= alignof (max_align_t));
    return malloc(((void) align, size));
#endif
}
