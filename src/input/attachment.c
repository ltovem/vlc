// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * attachment.c: Input attachments
 *****************************************************************************
 * Copyright (C) 1999-2020 VLC authors and VideoLAN
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_input.h>
#include <vlc_atomic.h>

struct input_attachment_priv
{
    input_attachment_t a;
    vlc_atomic_rc_t rc;
};

static struct input_attachment_priv* input_attachment_priv( input_attachment_t* a )
{
    return container_of( a, struct input_attachment_priv, a );
}

void vlc_input_attachment_Release( input_attachment_t *a )
{
    if( !a )
        return;

    struct input_attachment_priv* p = input_attachment_priv( a );

    if( !vlc_atomic_rc_dec( &p->rc ) )
        return;

    free( a->p_data );
    free( a->psz_description );
    free( a->psz_mime );
    free( a->psz_name );
    free( p );
}

input_attachment_t *vlc_input_attachment_New( const char *psz_name,
                                              const char *psz_mime,
                                              const char *psz_description,
                                              const void *p_data,
                                              size_t i_data )
{
    struct input_attachment_priv *a = (struct input_attachment_priv *)malloc( sizeof (*a) );
    if( unlikely(a == NULL) )
        return NULL;

    vlc_atomic_rc_init( &a->rc );
    a->a.psz_name = strdup( psz_name ? psz_name : "" );
    a->a.psz_mime = strdup( psz_mime ? psz_mime : "" );
    a->a.psz_description = strdup( psz_description ? psz_description : "" );
    a->a.i_data = i_data;
    a->a.p_data = malloc( i_data );
    if( i_data > 0 && likely(a->a.p_data != NULL) )
        memcpy( a->a.p_data, p_data, i_data );

    if( unlikely(a->a.psz_name == NULL || a->a.psz_mime == NULL
              || a->a.psz_description == NULL || (i_data > 0 && a->a.p_data == NULL)) )
    {
        vlc_input_attachment_Release( &a->a );
        return NULL;
    }
    return &a->a;
}

input_attachment_t *vlc_input_attachment_Hold( input_attachment_t *a )
{
    struct input_attachment_priv* p = input_attachment_priv( a );
    vlc_atomic_rc_inc( &p->rc );
    return a;
}
