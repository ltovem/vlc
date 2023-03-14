/*****************************************************************************
 * writev.c: POSIX writev() replacement
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

ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
    if (iovcnt == 1)
        return write(fd, iov->iov_base, iov->iov_len);

    size_t size = 0;

    for (int i = 0; i < iovcnt; i++) {
        /* TODO: use ckd_add() */
        size += iov[i].iov_len;

        if (iov[i].iov_len > size) {
            errno = EINVAL;
            return -1;
        }
    }

    void *buf = malloc(size ? size : 1);
    if (buf == NULL) {
        errno = ENOBUFS;
        return -1;
    }

    unsigned char *ptr = buf;

    for (int i = 0; i < iovcnt; i++) {
        memcpy(ptr, iov->iov_base, iov->iov_len);
        ptr += iov->iov_len;
        iov++;
    }

    ssize_t bytes = write(fd, buf, size);
    int saved_errno = errno;

    free(buf);
    errno = saved_errno;
    return bytes;
}
