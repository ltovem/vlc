/*****************************************************************************
 * snapshot.h : vout internal snapshot
 *****************************************************************************
 * Copyright (C) 2009 Laurent Aimar
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef LIBVLC_VOUT_INTERNAL_SNAPSHOT_H
#define LIBVLC_VOUT_INTERNAL_SNAPSHOT_H

#include <vlc_picture.h>

typedef struct vout_snapshot vout_snapshot_t;

/* */
vout_snapshot_t *vout_snapshot_New(void);
void vout_snapshot_Destroy(vout_snapshot_t *);

void vout_snapshot_End(vout_snapshot_t *);

/* */
picture_t *vout_snapshot_Get(vout_snapshot_t *, vlc_tick_t timeout);

/**
 * It tells if they are pending snapshot request
 */
bool vout_snapshot_IsRequested(vout_snapshot_t *);

/**
 * It set the picture used to create the snapshots.
 *
 * The given picture is cloned.
 * If p_fmt is non NULL it will override the format of the p_picture (mainly
 * used because of aspect/crop problems).
 */
void vout_snapshot_Set(vout_snapshot_t *, const video_format_t *, picture_t *);

/**
 * This function will return the directory used for snapshots
 */
char *vout_snapshot_GetDirectory(void);

typedef struct {
    bool is_sequential;
    int  sequence;
    char *path;
    char *format;
    char *prefix_fmt;
} vout_snapshot_save_cfg_t;

/**
 * This function will write an image to the disk an return the file name created.
 */
int vout_snapshot_SaveImage(char **name, int *sequential,
                            const block_t *image,
                            vout_thread_t *p_vout,
                            const vout_snapshot_save_cfg_t *cfg);

#endif
