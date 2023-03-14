/*****************************************************************************
 * variables.h: object variables typedefs
 *****************************************************************************
 * Copyright (C) 1999-2012 VLC authors and VideoLAN
 *
 * Authors: Samuel Hocevar <sam@zoy.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef LIBVLC_VARIABLES_H
# define LIBVLC_VARIABLES_H 1

# include <vlc_list.h>

struct vlc_res;

/**
 * Private LibVLC data for each object.
 */
typedef struct vlc_object_internals vlc_object_internals_t;

struct vlc_object_internals
{
    vlc_object_t *parent; /**< Parent object (or NULL) */
    const char *typename; /**< Object type human-readable name */

    /* Object variables */
    void           *var_root;
    vlc_mutex_t     var_lock;

    /* Object resources */
    struct vlc_res *resources;
};

# define vlc_internals(o) ((o)->priv)
# define vlc_externals(priv) (abort(), (void *)(priv))

extern void var_DestroyAll( vlc_object_t * );

/**
 * Return a list of all variable names
 *
 * There is no warranty that the returned variables will be still alive after
 * the return of this function.
 *
 * @return a NULL terminated list of char *, each elements and the return value
 * must be freed by the caller
 */
char **var_GetAllNames(vlc_object_t *);

#endif
