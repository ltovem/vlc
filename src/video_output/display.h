// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * display.h: "vout display" management
 *****************************************************************************
 * Copyright (C) 2009 Laurent Aimar
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *****************************************************************************/

#include "vout_wrapper.h"

void vout_UpdateDisplaySourceProperties(vout_display_t *vd, const video_format_t *, const vlc_rational_t *forced_dar);
void VoutFixFormatAR(video_format_t *);
