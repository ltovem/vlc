/*****************************************************************************
 * readv.c: POSIX readv() replacement
 *****************************************************************************
 * Copyright © 2022 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
{
    if (iovcnt == 1)
        return read(fd, iov->iov_base, iov->iov_len);

    /*
     * The whole point of readv() is that multiple consecutive reads are *not*
     * generally equivalent to one read of the same total size. We have to read
     * all at once to preserve atomicity and packet boundaries.
     */
    size_t size = 0;

    for (int i = 0; i < iovcnt; i++) {
        /* TODO: use ckd_add() */
        size += iov[i].iov_len;

        if (iov[i].iov_len > size) {
            errno = EINVAL;
            return -1;
        }
    }

    unsigned char *buf = malloc(size ? size : 1);
    if (buf == NULL) {
        errno = ENOBUFS;
        return -1;
    }

    ssize_t ret = read(fd, buf, size);
    int saved_errno = errno;

    for (ssize_t len = ret; len > 0; iov++) {
        size_t copy = iov->iov_len;

        if (copy > (size_t)len)
            copy = len;

        memcpy(iov->iov_base, buf, copy);
        buf += copy;
        len -= copy;
    }

    free(buf);
    errno = saved_errno;
    return ret;
}
