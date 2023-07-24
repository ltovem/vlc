/*****************************************************************************
 * yuvp.c: YUVP to YUVA/RGBA chroma converter
 *****************************************************************************
 * Copyright (C) 2008 VLC authors and VideoLAN
 *
 * Authors: Laurent Aimar < fenrir @ videolan.org >
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
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

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_filter.h>
#include <vlc_picture.h>
#include <assert.h>

/* TODO:
 *  Add anti-aliasing support (specially for DVD where only 4 colors are used)
 */

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int  Open ( filter_t * );

vlc_module_begin ()
    set_description( N_("YUVP converter") )
    set_callback_video_converter( Open, 10 )
vlc_module_end ()

/****************************************************************************
 * Local prototypes
 ****************************************************************************/
static vlc_palette_color Yuv2Rgb( int y1, int u1, int v1, uint8_t a1 );

VIDEO_FILTER_WRAPPER( Convert )

/*****************************************************************************
 * Open: probe the filter and return score
 *****************************************************************************/
static int Open( filter_t *p_filter )
{
    /* It only supports YUVP to YUVA/RGBA without scaling
     * (if scaling is required another filter can do it) */
    if( p_filter->fmt_in.video.i_chroma != VLC_CODEC_YUVP ||
        ( p_filter->fmt_out.video.i_chroma != VLC_CODEC_YUVA &&
          p_filter->fmt_out.video.i_chroma != VLC_CODEC_RGBA &&
          p_filter->fmt_out.video.i_chroma != VLC_CODEC_ARGB &&
          p_filter->fmt_out.video.i_chroma != VLC_CODEC_BGRA &&
          p_filter->fmt_out.video.i_chroma != VLC_CODEC_ABGR) ||
        p_filter->fmt_in.video.i_width  != p_filter->fmt_out.video.i_width ||
        p_filter->fmt_in.video.i_height != p_filter->fmt_out.video.i_height ||
        p_filter->fmt_in.video.orientation != p_filter->fmt_out.video.orientation )
    {
        return VLC_EGENERIC;
    }

    p_filter->ops = &Convert_ops;

    msg_Dbg( p_filter, "YUVP to %4.4s converter",
             (const char*)&p_filter->fmt_out.video.i_chroma );

    return VLC_SUCCESS;
}

static vlc_palette_color Yuv2ARGB(const uint8_t p[4])
{
    vlc_palette_color tmp = Yuv2Rgb(p[0], p[1], p[2], p[3]);
    return (vlc_palette_color) { .rgba = { tmp.rgba.a, tmp.rgba.r, tmp.rgba.g, tmp.rgba.b } };
}

static vlc_palette_color Yuv2RGBA(const uint8_t p[4])
{
    vlc_palette_color tmp = Yuv2Rgb(p[0], p[1], p[2], p[3]);
    return (vlc_palette_color) { .rgba = { tmp.rgba.r, tmp.rgba.g, tmp.rgba.b, tmp.rgba.a } };
}

static vlc_palette_color Yuv2BGRA(const uint8_t p[4])
{
    vlc_palette_color tmp = Yuv2Rgb(p[0], p[1], p[2], p[3]);
    return (vlc_palette_color) { .rgba = { tmp.rgba.b, tmp.rgba.g, tmp.rgba.r, tmp.rgba.a } };
}

static vlc_palette_color Yuv2ABGR(const uint8_t p[4])
{
    vlc_palette_color tmp = Yuv2Rgb(p[0], p[1], p[2], p[3]);
    return (vlc_palette_color) { .rgba = { tmp.rgba.a, tmp.rgba.b, tmp.rgba.g, tmp.rgba.r } };
}

/****************************************************************************
 * Filter: the whole thing
 ****************************************************************************/

static void Convert( filter_t *p_filter, picture_t *p_source,
                                           picture_t *p_dest )
{
    const video_palette_t *p_yuvp = p_filter->fmt_in.video.p_palette;

    assert( p_yuvp != NULL );
    assert( p_filter->fmt_in.video.i_chroma == VLC_CODEC_YUVP );
    assert( p_filter->fmt_in.video.i_width == p_filter->fmt_out.video.i_width );
    assert( p_filter->fmt_in.video.i_height == p_filter->fmt_out.video.i_height );

    if( p_filter->fmt_out.video.i_chroma == VLC_CODEC_YUVA )
    {
        for( unsigned int y = 0; y < p_filter->fmt_in.video.i_height; y++ )
        {
            const uint8_t *p_line = &p_source->p->p_pixels[y*p_source->p->i_pitch];
            uint8_t *p_y = &p_dest->Y_PIXELS[y*p_dest->Y_PITCH];
            uint8_t *p_u = &p_dest->U_PIXELS[y*p_dest->U_PITCH];
            uint8_t *p_v = &p_dest->V_PIXELS[y*p_dest->V_PITCH];
            uint8_t *p_a = &p_dest->A_PIXELS[y*p_dest->A_PITCH];

            for( unsigned int x = 0; x < p_filter->fmt_in.video.i_width; x++ )
            {
                const int v = p_line[x];

                if( v > p_yuvp->i_entries )  /* maybe assert ? */
                    continue;

                p_y[x] = p_yuvp->palette[v][0];
                p_u[x] = p_yuvp->palette[v][1];
                p_v[x] = p_yuvp->palette[v][2];
                p_a[x] = p_yuvp->palette[v][3];
            }
        }
    }
    else
    {
        video_palette_t rgbp;
        vlc_palette_color (*conv)(const uint8_t p[4]);

        switch( p_filter->fmt_out.video.i_chroma )
        {
            case VLC_CODEC_ARGB: conv = Yuv2ARGB; break;
            case VLC_CODEC_RGBA: conv = Yuv2RGBA; break;
            case VLC_CODEC_BGRA: conv = Yuv2BGRA; break;
            case VLC_CODEC_ABGR: conv = Yuv2ABGR; break;
            default:
                vlc_assert_unreachable();
        }
        /* Create a RGBA palette */
        rgbp.i_entries = p_yuvp->i_entries;
        for( int i = 0; i < p_yuvp->i_entries; i++ )
        {
            if( p_yuvp->palette[i][3] == 0 )
            {
                memset( rgbp.palette[i], 0, sizeof( rgbp.palette[i] ) );
                continue;
            }
            vlc_palette_color pi = conv( p_yuvp->palette[i] );
            memcpy( rgbp.palette[i], &pi, 4 );
        }

        for( unsigned int y = 0; y < p_filter->fmt_in.video.i_height; y++ )
        {
            const uint8_t *p_line = &p_source->p->p_pixels[y*p_source->p->i_pitch];
            uint8_t *p_pixels = &p_dest->p->p_pixels[y*p_dest->p->i_pitch];

            for( unsigned int x = 0; x < p_filter->fmt_in.video.i_width; x++ )
            {
                const int v = p_line[x];

                if( v >= rgbp.i_entries )  /* maybe assert ? */
                    continue;

                p_pixels[4*x+0] = rgbp.palette[v][0];
                p_pixels[4*x+1] = rgbp.palette[v][1];
                p_pixels[4*x+2] = rgbp.palette[v][2];
                p_pixels[4*x+3] = rgbp.palette[v][3];
            }
        }

    }
}

/* FIXME copied from blend.c */
static inline uint8_t vlc_uint8( int v )
{
    if( v > 255 )
        return 255;
    else if( v < 0 )
        return 0;
    return v;
}
static vlc_palette_color Yuv2Rgb( int y1, int u1, int v1, uint8_t a1 )
{
    /* macros used for YUV pixel conversions */
#   define SCALEBITS 10
#   define ONE_HALF  (1 << (SCALEBITS - 1))
#   define FIX(x)    ((int) ((x) * (1<<SCALEBITS) + 0.5))

    int y, cb, cr, r_add, g_add, b_add;

    cb = u1 - 128;
    cr = v1 - 128;
    r_add = FIX(1.40200*255.0/224.0) * cr + ONE_HALF;
    g_add = - FIX(0.34414*255.0/224.0) * cb
            - FIX(0.71414*255.0/224.0) * cr + ONE_HALF;
    b_add = FIX(1.77200*255.0/224.0) * cb + ONE_HALF;
    y = (y1 - 16) * FIX(255.0/219.0);
    return (vlc_palette_color) {
        .rgba = {
            .r = vlc_uint8( (y + r_add) >> SCALEBITS ),
            .g = vlc_uint8( (y + g_add) >> SCALEBITS ),
            .b = vlc_uint8( (y + b_add) >> SCALEBITS ),
            .a = a1,
        }
    };
#undef FIX
}
