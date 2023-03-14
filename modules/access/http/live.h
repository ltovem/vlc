/*****************************************************************************
 * live.h: HTTP read-only live stream declarations
 *****************************************************************************
 * Copyright (C) 2015 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

/**
 * \defgroup http_live Live streams
 * Trivial HTTP-based live streams
 * \ingroup http_res
 * @{
 */

struct vlc_http_resource;

struct vlc_http_resource *vlc_http_live_create(struct vlc_http_mgr *mgr,
                                               const char *uri, const char *ua,
                                               const char *ref);
block_t *vlc_http_live_read(struct vlc_http_resource *);

#define vlc_http_live_get_status vlc_http_res_get_status
#define vlc_http_live_get_redirect vlc_http_res_get_redirect
#define vlc_http_live_get_type vlc_http_res_get_type
#define vlc_http_live_destroy vlc_http_res_destroy

/** @} */
