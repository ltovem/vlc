// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * transcode.h: test for transcoding pipeline
 *****************************************************************************
 * Copyright (C) 2021 VideoLabs
 *
 * Author: Alexandre Janniaux <ajanni@videolabs.io>
 *****************************************************************************/

#include <vlc_fourcc.h>

#define TEST_FLAG_CONVERTER 0x01
#define TEST_FLAG_FILTER 0x02

struct transcode_scenario {
    const char *source;
    const char *sout;
    void (*decoder_setup)(decoder_t *);
    int (*decoder_decode)(decoder_t *, picture_t *);
    void (*encoder_setup)(encoder_t *);
    void (*encoder_close)(encoder_t *);
    void (*encoder_encode)(encoder_t *, picture_t *);
    void (*filter_setup)(filter_t *);
    void (*converter_setup)(filter_t *);
    void (*report_error)(sout_stream_t *);
    void (*report_output)(const vlc_frame_t *);
};


void transcode_scenario_init(void);
void transcode_scenario_wait(struct transcode_scenario *scenario);
void transcode_scenario_check(struct transcode_scenario *scenario);
extern size_t transcode_scenarios_count;
extern struct transcode_scenario transcode_scenarios[];
