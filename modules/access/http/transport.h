/*****************************************************************************
 * transport.h: HTTP/TLS TCP transport layer declarations
 *****************************************************************************
 * Copyright © 2015 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_HTTP_TRANSPORT_H
#define VLC_HTTP_TRANSPORT_H 1

#include <stddef.h>
#include <stdbool.h>

struct vlc_tls;
struct vlc_tls_client;

struct vlc_tls *vlc_https_connect(struct vlc_tls_client *creds,
                                  const char *name, unsigned port,
                                  bool *restrict two);
struct vlc_tls *vlc_https_connect_proxy(void *ctx,
                                        struct vlc_tls_client *creds,
                                        const char *name, unsigned port,
                                        bool *restrict two, const char *proxy);
bool vlc_http_port_blocked(unsigned port);

#endif
