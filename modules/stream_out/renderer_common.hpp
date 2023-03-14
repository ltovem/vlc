/*****************************************************************************
 * renderer_common.hpp : renderer helper functions
 *****************************************************************************
 * Copyright © 2014-2018 VideoLAN
 *
 * Authors: Adrien Maglo <magsoft@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *          Steve Lhomme <robux4@videolabs.io>
 *          Shaleen Jain <shaleen@jain.sh>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef RENDERER_COMMON_H
#define RENDERER_COMMON_H

#include <string>
#include <vector>

#include <vlc_common.h>
#include <vlc_sout.h>

#define PERF_TEXT N_( "Performance warning" )
#define PERF_LONGTEXT N_( "Display a performance warning when transcoding" )
#define AUDIO_PASSTHROUGH_TEXT N_( "Enable Audio passthrough" )
#define AUDIO_PASSTHROUGH_LONGTEXT N_( "Disable if your receiver does not support Dolby®." )
#define CONVERSION_QUALITY_TEXT N_( "Conversion quality" )
#define CONVERSION_QUALITY_LONGTEXT N_( "Change conversion speed or quality." )

#if defined (__ANDROID__) || defined (__arm__) || (defined (TARGET_OS_IPHONE) && TARGET_OS_IPHONE)
# define CONVERSION_QUALITY_DEFAULT CONVERSION_QUALITY_LOW
#else
# define CONVERSION_QUALITY_DEFAULT CONVERSION_QUALITY_MEDIUM
#endif

#define RENDERER_CFG_PREFIX "sout-renderer-"

#define add_renderer_opts(prefix) \
    add_integer(RENDERER_CFG_PREFIX "show-perf-warning", 1, \
            PERF_TEXT, PERF_LONGTEXT ) \
        change_private() \
    add_bool(prefix "audio-passthrough", false, \
            AUDIO_PASSTHROUGH_TEXT, AUDIO_PASSTHROUGH_LONGTEXT ) \
    add_integer(prefix "conversion-quality", CONVERSION_QUALITY_DEFAULT, \
                CONVERSION_QUALITY_TEXT, CONVERSION_QUALITY_LONGTEXT ); \
        change_integer_list(conversion_quality_list, conversion_quality_list_text)

static const char *const conversion_quality_list_text[] = {
    N_( "High (high quality and high bandwidth)" ),
    N_( "Medium (medium quality and medium bandwidth)" ),
    N_( "Low (low quality and low bandwidth)" ),
    N_( "Low CPU (low quality but high bandwidth)" ),
};

enum {
    CONVERSION_QUALITY_HIGH = 0,
    CONVERSION_QUALITY_MEDIUM = 1,
    CONVERSION_QUALITY_LOW = 2,
    CONVERSION_QUALITY_LOWCPU = 3,
};

static const int conversion_quality_list[] = {
    CONVERSION_QUALITY_HIGH,
    CONVERSION_QUALITY_MEDIUM,
    CONVERSION_QUALITY_LOW,
    CONVERSION_QUALITY_LOWCPU,
};

std::string
vlc_sout_renderer_GetVcodecOption(sout_stream_t *p_stream,
        std::vector<vlc_fourcc_t> codecs,
        vlc_fourcc_t *out_codec, const video_format_t *p_vid, int i_quality);

#endif /* RENDERER_COMMON_H */
