// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * window_presets.h : Variable names and strings of FFT window presets
 *****************************************************************************
 * Copyright (C) 2014 Ronald Wright
 *
 * Author: Ronald Wright <logiconcepts819@gmail.com>
 *****************************************************************************/

#ifndef VLC_VISUAL_WINDOW_PRESETS_H_
#define VLC_VISUAL_WINDOW_PRESETS_H_

/* Window functions supported by VLC. These are the typical window types used
 * in spectrum analyzers. */
#define NB_WINDOWS 5
static const char * const window_list[NB_WINDOWS] = {
    "none", "hann", "flattop", "blackmanharris", "kaiser",
};
static const char * const window_list_text[NB_WINDOWS] = {
    N_("None"), N_("Hann"), N_("Flat Top"), N_("Blackman-Harris"), N_("Kaiser"),
};

#endif /* include-guard */
