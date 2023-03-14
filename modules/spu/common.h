/*****************************************************************************
 * common.h : common spu defines
 *****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#define OPACITY_TEXT N_("Opacity")
#define OPACITY_LONGTEXT N_("Opacity (inverse of transparency), " \
  "from 0 for fully transparent to 255 for fully opaque." )

#define POSX_TEXT N_("X offset")
#define POSY_TEXT N_("Y offset")
#define POSX_LONGTEXT N_("X offset, from top-left, or from relative position." )
#define POSY_LONGTEXT N_("Y offset, from top-left, or from relative position." )

#define POS_TEXT N_("Position")
#define POS_LONGTEXT N_( \
  "Set the position on the video " \
  "(-1=absolute, 0=center, 1=left, 2=right, 4=top, 8=bottom; you can " \
  "also use combinations of these values, e.g. 6 = top-right).")

/* Excluding absolute, these values correspond to SUBPICTURE_ALIGN_* flags */
static const int pi_pos_values[] = { -1, 0, 1, 2, 4, 8, 5, 6, 9, 10 };
static const char *const ppsz_pos_descriptions[] =
{ N_("Absolute"),
  N_("Center"), N_("Left"), N_("Right"), N_("Top"), N_("Bottom"),
  N_("Top-Left"), N_("Top-Right"), N_("Bottom-Left"), N_("Bottom-Right") };
