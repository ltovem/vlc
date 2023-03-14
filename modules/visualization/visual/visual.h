/*****************************************************************************
 * visual.h : Header for the visualisation system
 *****************************************************************************
 * Copyright (C) 2002-2009 VLC authors and VideoLAN
 *
 * Authors: Clément Stenac <zorglub@via.ecp.fr>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

typedef struct visual_effect_t visual_effect_t;
typedef int (*visual_run_t)(visual_effect_t *, vlc_object_t *,
                            const block_t *, picture_t *);
typedef void (*visual_free_t)(void *);

struct visual_effect_t
{
    visual_run_t pf_run;
    visual_free_t pf_free;
    void *     p_data; /* The effect stores whatever it wants here */
    int        i_width;
    int        i_height;
    int        i_nb_chans;

    /* Channels index */
    int        i_idx_left;
    int        i_idx_right;
};

extern const struct visual_cb_t
{
    char name[16];
    visual_run_t run_cb;
    visual_free_t free_cb;
} effectv[];
extern const unsigned effectc;
