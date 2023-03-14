/*****************************************************************************
 * outfile.h: HTTP write-only file
 *****************************************************************************
 * Copyright (C) 2015, 2020 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#include <stdint.h>

/**
 * \defgroup http_outfile Output files
 * HTTP write-only files
 * \ingroup http
 * @{
 */

struct vlc_http_mgr;
struct vlc_http_outfile;

/**
 * Creates an HTTP output file.
 *
 * @param mgr HTTP connection manager
 * @param url URL of the file to write
 * @param ua user-agent string (NULL to ignore)
 * @param user username for authentication (NULL to skip)
 * @param pwd password for authentication (NULL to skip)
 *
 * @return an HTTP resource object pointer, or NULL on error
 */
struct vlc_http_outfile *vlc_http_outfile_create(struct vlc_http_mgr *mgr,
    const char *url, const char *ua, const char *user, const char *pwd);

/**
 * Writes data.
 */
ssize_t vlc_http_outfile_write(struct vlc_http_outfile *, block_t *b);
int vlc_http_outfile_close(struct vlc_http_outfile *);

/** @} */
