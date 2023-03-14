/*****************************************************************************
 * dirfd.c: POSIX dirfd replacement
 *****************************************************************************
 * Copyright © 2009-2011 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <dirent.h>
#include <errno.h>

int (dirfd) (DIR *dir)
{
#ifdef dirfd
    return dirfd (dir);
#else
    (void) dir;
# ifdef ENOTSUP
    errno = ENOTSUP;
# endif
    return -1;
#endif
}
