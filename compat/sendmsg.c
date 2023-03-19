// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * sendmsg.c: POSIX sendmsg() replacement
 *****************************************************************************
 * Copyright © 2017 VLC authors and VideoLAN
 * Copyright © 2016 Rémi Denis-Courmont
 *
 * Authors: Rémi Denis-Courmont
 *          Dennis Hamester <dhamester@jusst.de>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef _WIN32
# include <errno.h>
# include <stdlib.h>
# include <winsock2.h>

ssize_t sendmsg(int fd, const struct msghdr *msg, int flags)
{
    if (msg->msg_controllen != 0)
    {
        errno = ENOSYS;
        return -1;
    }

    if (msg->msg_iovlen > IOV_MAX)
    {
        errno = EINVAL;
        return -1;
    }

    WSABUF *buf = malloc(msg->msg_iovlen * sizeof (*buf));
    if (buf == NULL)
        return -1;

    for (unsigned i = 0; i < msg->msg_iovlen; i++)
    {
        buf[i].len = msg->msg_iov[i].iov_len;
        buf[i].buf = (void *)msg->msg_iov[i].iov_base;
    }

    DWORD sent;

    int ret = WSASendTo(fd, buf, msg->msg_iovlen, &sent, flags,
                        msg->msg_name, msg->msg_namelen, NULL, NULL);
    free(buf);

    if (ret == 0)
        return sent;

    int err = WSAGetLastError();
    switch (err)
    {
        case WSAEWOULDBLOCK:
            errno = EAGAIN;
            break;
        case WSA_NOT_ENOUGH_MEMORY:
            errno = ENOMEM;
            break;
        case WSAEAFNOSUPPORT:
            errno = EAFNOSUPPORT;
            break;
        case WSAENOBUFS:
            errno = ENOBUFS;
            break;
        case WSAEINPROGRESS:
            errno = EINPROGRESS;
            break;
        case WSAEINTR:
            errno = EINTR;
            break;
        case WSAEBADF:
            errno = EBADF;
            break;
        case WSAEACCES:
            errno = EACCES;
            break;
        case WSAEFAULT:
            errno = EFAULT;
            break;
        case WSAEINVAL:
            errno = EINVAL;
            break;
        case WSAEMFILE:
            errno = EMFILE;
            break;
        case WSAEALREADY:
            errno = EALREADY;
            break;
        case WSAECONNRESET:
            errno = ECONNRESET;
            break;
        default:
            errno = err;
            break;
    }
    return -1;
}

#else
#error sendmsg not implemented on your platform!
#endif
