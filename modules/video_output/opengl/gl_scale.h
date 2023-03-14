/*****************************************************************************
+ * gl_scale.h
+ *****************************************************************************
+ * Copyright (C) 2021 VLC authors and VideoLAN
+ *
+ * SPDX-License-Identifier: LGPL-2.1-or-later
+ *****************************************************************************/

#ifndef VLC_GL_SCALE_H
#define VLC_GL_SCALE_H

enum vlc_gl_scale {
    VLC_GLSCALE_BUILTIN,
    VLC_GLSCALE_SPLINE16,
    VLC_GLSCALE_SPLINE36,
    VLC_GLSCALE_SPLINE64,
    VLC_GLSCALE_MITCHELL,
    VLC_GLSCALE_BICUBIC,
    VLC_GLSCALE_EWA_LANCZOS,
    VLC_GLSCALE_NEAREST,
    VLC_GLSCALE_BILINEAR,
    VLC_GLSCALE_GAUSSIAN,
    VLC_GLSCALE_LANCZOS,
    VLC_GLSCALE_GINSENG,
    VLC_GLSCALE_EWA_GINSENG,
    VLC_GLSCALE_EWA_HANN,
    VLC_GLSCALE_CATMULL_ROM,
    VLC_GLSCALE_ROBIDOUX,
    VLC_GLSCALE_ROBIDOUXSHARP,
    VLC_GLSCALE_EWA_ROBIDOUX,
    VLC_GLSCALE_EWA_ROBIDOUXSHARP,
    VLC_GLSCALE_SINC,
    VLC_GLSCALE_EWA_JINC,
};

static const int vlc_glscale_values[] = {
    VLC_GLSCALE_BUILTIN,
    VLC_GLSCALE_SPLINE16,
    VLC_GLSCALE_SPLINE36,
    VLC_GLSCALE_SPLINE64,
    VLC_GLSCALE_MITCHELL,
    VLC_GLSCALE_BICUBIC,
    VLC_GLSCALE_EWA_LANCZOS,
    VLC_GLSCALE_NEAREST,
    VLC_GLSCALE_BILINEAR,
    VLC_GLSCALE_GAUSSIAN,
    VLC_GLSCALE_LANCZOS,
    VLC_GLSCALE_GINSENG,
    VLC_GLSCALE_EWA_GINSENG,
    VLC_GLSCALE_EWA_HANN,
    VLC_GLSCALE_CATMULL_ROM,
    VLC_GLSCALE_ROBIDOUX,
    VLC_GLSCALE_ROBIDOUXSHARP,
    VLC_GLSCALE_EWA_ROBIDOUX,
    VLC_GLSCALE_EWA_ROBIDOUXSHARP,
    VLC_GLSCALE_SINC,
    VLC_GLSCALE_EWA_JINC,
};

static const char *const vlc_glscale_text[] = {
    "Built-in / fixed function (fast)",
    "Spline 2 taps",
    "Spline 3 taps (recommended upscaler)",
    "Spline 4 taps",
    "Mitchell-Netravali (recommended downscaler)",
    "Bicubic",
    "Jinc / EWA Lanczos 3 taps (high quality, slow)",
    "Nearest neighbor",
    "Bilinear",
    "Gaussian",
    "Lanczos 3 taps",
    "Ginseng 3 taps",
    "EWA Ginseng",
    "EWA Hann",
    "Catmull-Rom",
    "Robidoux",
    "RobidouxSharp",
    "EWA Robidoux",
    "EWA RobidouxSharp",
    "Unwindowed sinc (clipped)",
    "Unwindowed EWA Jinc (clipped)",
};

#define VLC_GL_UPSCALER_TEXT "OpenGL upscaler"
#define VLC_GL_UPSCALER_LONGTEXT "Upscaler filter to apply during rendering"

#define VLC_GL_DOWNSCALER_TEXT "OpenGL downscaler"
#define VLC_GL_DOWNSCALER_LONGTEXT "Downscaler filter to apply during rendering"

#define add_glscale_opts() \
    set_section(N_("Scaling"), NULL) \
    add_integer("gl-upscaler", VLC_GLSCALE_BUILTIN, VLC_GL_UPSCALER_TEXT, \
                VLC_GL_UPSCALER_LONGTEXT) \
        change_integer_list(vlc_glscale_values, vlc_glscale_text) \
    add_integer("gl-downscaler", VLC_GLSCALE_BUILTIN, VLC_GL_DOWNSCALER_TEXT, \
                VLC_GL_DOWNSCALER_LONGTEXT) \
        change_integer_list(vlc_glscale_values, vlc_glscale_text) \

#endif
