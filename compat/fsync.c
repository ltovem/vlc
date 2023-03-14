/*****************************************************************************
 * fsync.c: POSIX fsync() replacement
 *****************************************************************************
 * Copyright © 2011 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef _WIN32
# include <io.h>
#endif

int fsync (int fd)
{
#if defined(_WIN32)
    /* WinCE can use FlushFileBuffers() but it operates on file handles */
    return _commit (fd);
#else
# warning fsync() not implemented!
    return 0;
#endif
}
