/*****************************************************************************
 * connmgr.h: HTTP/TLS VLC connection manager declaration
 *****************************************************************************
 * Copyright © 2015 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

/**
 * \defgroup http HTTP
 * \ingroup net
 * Hyper-Text Transfer Protocol
 * \defgroup http_connmgr Connection manager
 * HTTP connection management
 * \ingroup http
 * @{
 * \file connmgr.h
 */

struct vlc_http_mgr;
struct vlc_http_msg;
struct vlc_http_cookie_jar_t;

/**
 * Sends an HTTP request
 *
 * Sends an HTTP request, by either reusing an existing HTTP connection or
 * establishing a new one. If successful, the initial HTTP response header is
 * returned.
 *
 * @param mgr HTTP connection manager
 * @param https whether to use HTTPS (true) or unencrypted HTTP (false)
 * @param host name of authoritative HTTP server to send the request to
 * @param port TCP server port number, or 0 for the default port number
 * @param req HTTP request header to send
 * @param idempotent whether the request is idempotent
 * @param payload whether the request will carry a payload
 *
 * @return The initial HTTP response header, or NULL in case of failure.
 */
struct vlc_http_msg *vlc_http_mgr_request(struct vlc_http_mgr *mgr, bool https,
                                          const char *host, unsigned port,
                                          const struct vlc_http_msg *req,
                                          bool idempotent, bool payload);

struct vlc_http_cookie_jar_t *vlc_http_mgr_get_jar(struct vlc_http_mgr *);

/**
 * Creates an HTTP connection manager
 *
 * Allocates an HTTP client connections manager.
 *
 * @param obj parent VLC object
 * @param jar HTTP cookies jar (NULL to disable cookies)
 */
struct vlc_http_mgr *vlc_http_mgr_create(vlc_object_t *obj,
                                         struct vlc_http_cookie_jar_t *jar);

/**
 * Destroys an HTTP connection manager
 *
 * Deallocates an HTTP client connections manager created by
 * vlc_http_msg_destroy(). Any remaining connection is closed and destroyed.
 */
void vlc_http_mgr_destroy(struct vlc_http_mgr *mgr);

/** @} */
