/*****************************************************************************
 * picture.h: picture internals
 *****************************************************************************
 * Copyright (C) 2015 Remi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#include <stdatomic.h>
#include <stddef.h>

#include <vlc_picture.h>
struct vlc_ancillary;

typedef struct
{
    picture_t picture;
    struct
    {
        void (*destroy)(picture_t *);
        void *opaque;
    } gc;

    /** Private ancillary struct. Don't use it directly, but use it via
     * picture_AttachAncillary() and picture_GetAncillary(). */
    struct vlc_ancillary **ancillaries;
} picture_priv_t;

void *picture_Allocate(int *, size_t);
void picture_Deallocate(int, void *, size_t);

picture_t * picture_InternalClone(picture_t *, void (*pf_destroy)(picture_t *), void *);
