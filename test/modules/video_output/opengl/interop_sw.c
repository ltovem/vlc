/*****************************************************************************
 * interop_sw.c: test for the software interop GL code
 *****************************************************************************
 * Copyright (C) 2023 VideoLabs, VideoLAN and VLC Authors
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifndef VLC_TEST_OPENGL_API
# error "Define VLC_TEST_OPENGL_API to the VLC_OPENGL API to use"
#endif

/* Define a builtin module for mocked parts */
#define MODULE_NAME test_opengl
#undef VLC_DYNAMIC_PLUGIN

#include "../../../libvlc/test.h"
#include "../../../../lib/libvlc_internal.h"
#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_codec.h>
#include <vlc_opengl.h>
#include <vlc_filter.h>
#include <vlc_modules.h>
#include <vlc_vout_display.h>
//#include <vlc_subpicture.h>

#include "../../../../modules/video_output/opengl/gl_api.h"
#include "../../../../modules/video_output/opengl/interop.h"

static_assert(
    VLC_TEST_OPENGL_API == VLC_OPENGL ||
    VLC_TEST_OPENGL_API == VLC_OPENGL_ES2,
    "VLC_TEST_OPENGL_API must be assigned to VLC_OPENGL or VLC_OPENGL_ES2");

const char vlc_module_name[] = MODULE_STRING;

int main( int argc, char **argv )
{
    (void)argc; (void)argv;
    test_init();

    const char * const vlc_argv[] = {
        "-vvv", "--aout=dummy", "--text-renderer=dummy", "--dec-dev=none",
    };

    libvlc_instance_t *vlc = libvlc_new(ARRAY_SIZE(vlc_argv), vlc_argv);
    vlc_object_t *root = &vlc->p_libvlc_int->obj;

    const char *cap =
        (VLC_TEST_OPENGL_API == VLC_OPENGL)     ? "opengl offscreen" :
        (VLC_TEST_OPENGL_API == VLC_OPENGL_ES2) ? "opengl es2 offscreen" :
        NULL;
    assert(cap != NULL);

    module_t **providers;
    size_t strict_matches;
    ssize_t provider_available = vlc_module_match(
        cap, NULL, false, &providers, &strict_matches);
    (void)strict_matches;
    free(providers);

    if (provider_available <= 0)
    {
        libvlc_release(vlc);
        return 77;
    }

    struct vlc_decoder_device *device =
        vlc_decoder_device_Create(root, NULL);
    vlc_gl_t *gl = vlc_gl_CreateOffscreen(
        root, device, 3, 3, VLC_TEST_OPENGL_API, NULL, NULL);
    if (device != NULL)
        vlc_decoder_device_Release(device);

    if (gl == NULL)
    {
        libvlc_release(vlc);
        return 77;
    }

    video_format_t fmt;
    video_format_Init(&fmt, VLC_CODEC_RGB32);

    struct
    {
        vlc_fourcc_t fcc;
        uint32_t rmask;
        uint32_t gmask;
        uint32_t bmask;
    } inputs[] = {
        { VLC_CODEC_RGB32, 0xff000000, 0x00ff0000, 0x0000ff00 },
        { VLC_CODEC_RGB32, 0x0000ff00, 0x00ff0000, 0xff000000 },
        { VLC_CODEC_RGB32, 0x00ff0000, 0x0000ff00, 0x000000ff },
        { VLC_CODEC_RGB32, 0x000000ff, 0x0000ff00, 0x00ff0000 },
        { VLC_CODEC_RGB16, 0, 0, 0 },
        { VLC_CODEC_RGB15, 0, 0, 0 },
        { VLC_CODEC_RGB8,  0, 0, 0 },
        { VLC_CODEC_RGBA,  0, 0, 0 },
        { VLC_CODEC_ARGB,  0, 0, 0 },
        /* can not test YUV as driver returns 0 surfaces */
    }, outputs[] = {
        { VLC_CODEC_RGB32, 0xff000000, 0x00ff0000, 0x0000ff00 },
        { VLC_CODEC_RGB32, 0x0000ff00, 0x00ff0000, 0xff000000 },
        { VLC_CODEC_RGBA,  0, 0, 0 },
        { VLC_CODEC_BGRA,  0, 0, 0 },
        { VLC_CODEC_RGB24, 0xff0000, 0x00ff00, 0x0000ff },
        { VLC_CODEC_RGB24, 0x0000ff, 0x00ff00, 0xff0000 },
    };

    for(size_t i=0; i<ARRAY_SIZE(inputs); i++)
    {
        fmt.i_chroma = inputs[i].fcc;
        fmt.i_rmask = inputs[i].rmask;
        fmt.i_gmask = inputs[i].gmask;
        fmt.i_bmask = inputs[i].bmask;

        fprintf(stderr,"requesting interop for chroma %4.4s r%x g%x b%x\n",
                        (const char*) &fmt.i_chroma,
                        fmt.i_rmask, fmt.i_gmask, fmt.i_bmask);

        struct vlc_gl_interop *interop = vlc_gl_interop_New(gl, NULL, &fmt);
        assert(interop);
        fprintf(stderr,"loaded interop requesting input as %4.4s r%x g%x b%x\n",
                       (const char*) &interop->fmt_in.i_chroma,
                       interop->fmt_in.i_rmask, interop->fmt_in.i_gmask, interop->fmt_in.i_bmask);

        size_t j;
        for(j=0; j<ARRAY_SIZE(outputs); j++)
        {
            if(outputs[j].fcc == interop->fmt_in.i_chroma &&
               outputs[j].rmask == interop->fmt_in.i_rmask &&
               outputs[j].gmask == interop->fmt_in.i_gmask &&
               outputs[j].bmask == interop->fmt_in.i_bmask)
                break;
        }


        assert(j != ARRAY_SIZE(outputs));

        vlc_gl_interop_Delete(interop);
    }

    vlc_gl_Delete(gl);

    libvlc_release(vlc);
    return 0;
}
