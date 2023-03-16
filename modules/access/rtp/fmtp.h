// SPDX-License-Identifier: LGPL-2.1-or-later
/**
 * @file fmtp.h
 * @brief SDP format parameters (fmtp) helpers
 */
/*****************************************************************************
 * Copyright © 2022 Rémi Denis-Courmont
 ****************************************************************************/

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline
const char *vlc_sdp_fmtp_get_str(const struct vlc_sdp_pt *desc,
                                 const char *name, size_t *restrict lenp)
{
    const char *p = desc->parameters;
    size_t namelen = strlen(name);

    while (p != NULL) {
        p += strspn(p, " ");

        if (strncmp(p, name, namelen) == 0 && p[namelen] == '=') {
            p += namelen + 1;
            *lenp = strcspn(p, ";");
            return p;
        }

        p = strchr(p, ';');
        if (p != NULL)
            p++;
    }

    return NULL;
}

static
int vlc_sdp_fmtp_get_ull(const struct vlc_sdp_pt *desc, const char *name,
                         unsigned long long *restrict res)
{
    size_t len;
    const char *n = vlc_sdp_fmtp_get_str(desc, name, &len);
    if (n == NULL)
        return -ENOENT;
    if (len == 0)
        return -EINVAL;

    char *end;
    unsigned long long ull = strtoull(n, &end, 10);
    if (end != n + len)
        return -EINVAL;

    *res = ull;
    return 0;
}

static inline
int vlc_sdp_fmtp_get_u16(const struct vlc_sdp_pt *desc, const char *name,
                         uint16_t *restrict res)
{
    unsigned long long ull;
    int err = vlc_sdp_fmtp_get_ull(desc, name, &ull);
    if (err == 0) {
        if (ull >> 16)
            return -ERANGE;

        *res = ull;
    }
    return err;
}

static inline
int vlc_sdp_fmtp_get_u8(const struct vlc_sdp_pt *desc, const char *name,
                        uint8_t *restrict res)
{
    unsigned long long ull;
    int err = vlc_sdp_fmtp_get_ull(desc, name, &ull);
    if (err == 0) {
        if (ull >> 8)
            return -ERANGE;

        *res = ull;
    }
    return err;
}

#define vlc_sdp_fmtp_get(desc, name, vp) \
    _Generic (*(vp), \
        uint16_t: vlc_sdp_fmtp_get_u16(desc, name, (uint16_t *)(vp)), \
        uint8_t:  vlc_sdp_fmtp_get_u8(desc, name, (uint8_t *)(vp)))

