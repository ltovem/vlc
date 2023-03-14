/*****************************************************************************
 * sdp_helper.h:
 *****************************************************************************
 * Copyright (C) 2002-2008 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_SOUT_SDP_H
#define VLC_SOUT_SDP_H

#include <stddef.h>

struct sockaddr;
struct vlc_memstream;

int vlc_sdp_Start(struct vlc_memstream *, vlc_object_t *obj,
                  const char *cfgpref,
                  const struct sockaddr *src, size_t slen,
                  const struct sockaddr *addr, size_t alen) VLC_USED;

#endif
