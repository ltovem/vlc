/*****************************************************************************
 * video_output.h: test for the video output pipeline
 *****************************************************************************
 * Copyright (C) 2021 VideoLabs
 *
 * Author: Alexandre Janniaux <ajanni@videolabs.io>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#include <vlc_fourcc.h>
#include <vlc_vout_display.h>


#define TEST_FLAG_CONVERTER 0x01
#define TEST_FLAG_FILTER 0x02

struct vout_scenario {
    const char *source;
    void (*decoder_setup)(decoder_t *);
    void (*decoder_decode)(decoder_t *, block_t *);
    int  (*display_setup)(vout_display_t *, video_format_t *,
                          struct vlc_video_context *);
    void (*display_prepare)(vout_display_t *, picture_t *);
    void (*display_display)(vout_display_t *, picture_t *);
    void (*filter_setup)(filter_t *);
    void (*converter_setup)(filter_t *);
};


void vout_scenario_init(void);
void vout_scenario_wait(struct vout_scenario *scenario);
extern size_t vout_scenarios_count;
extern struct vout_scenario vout_scenarios[];
