// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * video_window.h: window management for VLC video output
 *****************************************************************************
 * Copyright © 2014 Rémi Denis-Courmont
 *****************************************************************************/

struct vout_crop;

vlc_window_t *vout_display_window_New(vout_thread_t *);
void vout_display_window_Delete(vlc_window_t *);

void vout_display_ResizeWindow(vlc_window_t *,
                               const video_format_t *restrict original,
                               const vlc_rational_t *restrict dar,
                               const struct vout_crop *restrict crop,
                               const struct vout_display_placement *restrict dp);
void vout_display_window_SetMouseHandler(vlc_window_t *,
                                         vlc_mouse_event, void *);
