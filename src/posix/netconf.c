/*****************************************************************************
 * netconf.c : Network configuration
 *****************************************************************************
 * Copyright (C) 2013 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include <vlc_common.h>
#include <vlc_fs.h>
#include <vlc_network.h>
#include <vlc_spawn.h>

/**
 * Determines the network proxy server to use (if any).
 * @param url absolute URL for which to get the proxy server
 * @return proxy URL, NULL if no proxy or error
 */
char *vlc_getProxyUrl(const char *url)
{
    int fd[2];

    if (vlc_pipe(fd))
        return NULL;

    pid_t pid;
    int fdv[] = { -1, fd[1], STDERR_FILENO, -1 };
    const char *argv[] = { "proxy", url, NULL };

    if (vlc_spawnp(&pid, "proxy", fdv, argv))
        pid = -1;

    vlc_close(fd[1]);

    if (pid != -1)
    {
        char buf[1024];
        size_t len = 0;

        do
        {
             ssize_t val = read(fd[0], buf + len, sizeof (buf) - len);
             if (val <= 0)
                 break;
             len += val;
        }
        while (len < sizeof (buf));

        vlc_close(fd[0]);
        vlc_waitpid(pid);

        if (len >= 9 && !strncasecmp(buf, "direct://", 9))
            return NULL;

        char *end = memchr(buf, '\n', len);
        if (end != NULL)
        {
            *end = '\0';
            return strdup(buf);
        }
        /* Parse error: fallback (may be due to missing executable) */
    }
    else
        vlc_close(fd[0]);

    /* Fallback to environment variable */
    char *var = getenv("http_proxy");
    if (var != NULL)
        var = strdup(var);
    return var;
}
