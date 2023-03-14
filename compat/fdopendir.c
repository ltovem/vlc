/*****************************************************************************
 * fdopendir.c: POSIX fdopendir replacement
 *****************************************************************************
 * Copyright © 2011 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

DIR *fdopendir (int fd)
{
#ifdef F_GETFL
    /* Check read permission on file descriptor */
    int mode = fcntl (fd, F_GETFL);
    if (mode == -1 || (mode & O_ACCMODE) == O_WRONLY)
    {
        errno = EBADF;
        return NULL;
    }
#endif
    /* Check directory file type */
    struct stat st;
    if (fstat (fd, &st))
        return NULL;

    if (!S_ISDIR (st.st_mode))
    {
        errno = ENOTDIR;
        return NULL;
    }

    /* Try to open the directory through /proc where available.
     * Not all operating systems support this. Fix your libc! */
    char path[sizeof ("/proc/self/fd/") + 3 * sizeof (int)];
    sprintf (path, "/proc/self/fd/%u", fd);

    DIR *dir = opendir (path);
    if (dir != NULL)
    {
        close (fd);
        return dir;
    }

    /* Hide impossible errors for fdopendir() */
    switch (errno)
    {
        case EACCES:
#ifdef ELOOP
        case ELOOP:
#endif
        case ENAMETOOLONG:
        case ENOENT:
        case EMFILE:
        case ENFILE:
            errno = EIO;
    }
    return NULL;
}
