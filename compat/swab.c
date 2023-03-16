// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * swab.c: swab() replacement
 *****************************************************************************
 * Copyright © 2009 VLC authors and VideoLAN
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdint.h>

void swab( const void *p_src_, void *p_dst_, ssize_t n )
{
    const uint8_t *p_src = p_src_;
    uint8_t *p_dst = p_dst_;

    if( n < 0 )
        return;

    for( ssize_t i = 0; i < n-1; i += 2 )
    {
        uint8_t i_tmp = p_src[i+0];
        p_dst[i+0] = p_src[i+1];
        p_dst[i+1] = i_tmp;
    }
}

