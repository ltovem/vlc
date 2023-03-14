/*****************************************************************************
 * subpicture.h: Private subpicture definitions
 *****************************************************************************
 * Copyright (C) 2010 Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

struct subpicture_region_private_t {
    video_format_t fmt;
    picture_t      *p_picture;
};

subpicture_region_t * subpicture_region_NewInternal( const video_format_t *p_fmt );

subpicture_region_private_t *subpicture_region_private_New(video_format_t *);
void subpicture_region_private_Delete(subpicture_region_private_t *);

