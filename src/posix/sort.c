/******************************************************************************
 * sort.c: POSIX sort back-end
 ******************************************************************************
 * Copyright © 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <vlc_common.h>
#include <vlc_sort.h>

#ifdef HAVE_BROKEN_QSORT_R

struct compat_arg
{
    int (*compar)(const void *, const void *, void *);
    void *arg;
};

static int compar_compat(void *arg, const void *a, const void *b)
{
    struct compat_arg *compat_arg = arg;
    return compat_arg->compar(a, b, compat_arg->arg);
}

/* Follow the FreeBSD prototype */
void vlc_qsort(void *base, size_t nmemb, size_t size,
               int (*compar)(const void *, const void *, void *),
               void *arg)
{
    struct compat_arg compat_arg = {
        .compar = compar,
        .arg = arg
    };
    qsort_r(base, nmemb, size, &compat_arg, compar_compat);
}

#else

/* Follow the POSIX prototype */
void vlc_qsort(void *base, size_t nmemb, size_t size,
               int (*compar)(const void *, const void *, void *),
               void *arg)
{
    qsort_r(base, nmemb, size, compar, arg);
}
#endif
