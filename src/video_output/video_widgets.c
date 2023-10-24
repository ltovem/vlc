/*****************************************************************************
 * video_widgets.c : OSD widgets manipulation functions
 *****************************************************************************
 * Copyright (C) 2004-2010 VLC authors and VideoLAN
 *
 * Author: Yoann Peronneau <yoann@videolan.org>
 *         Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
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
#include <assert.h>

#include <vlc_common.h>
#include <vlc_vout.h>
#include <vlc_vout_osd.h>

#include <vlc_filter.h>

#include "vout_spuregion_helper.h"

#define STYLE_EMPTY 0
#define STYLE_FILLED 1

#define RGB_BLUE        0x2badde
#define RGB_ORANGE      0xf48b00
#define RGB_FILL        RGB_ORANGE

#define COL_TRANSPARENT 0
#define COL_WHITE       1
#define COL_FILL        2
#define COL_FILL_SHADE  3

typedef struct  {
    uint8_t r,g,b,a;
} vlc_palette_color;

#define SET_PALETTE_COLOR(id, rgb, alpha) \
    memcpy(&palette->palette[id], &(vlc_palette_color) { HEX2RGB(rgb), alpha }, 4);

static inline void SetPixelColor(uint8_t *p, const uint8_t color[4])
{
    p[0] = color[0];
    p[1] = color[1];
    p[2] = color[2];
    p[3] = color[3];
}

/**
 * Draws a rectangle at the given position in the region.
 * It may be filled (fill == STYLE_FILLED) or empty (fill == STYLE_EMPTY).
 */
static void DrawRect(subpicture_region_t *r, int fill, const uint8_t color[4],
                     int x1, int y1, int x2, int y2)
{
    uint8_t *p    = r->p_picture->p->p_pixels;
    int     pitch = r->p_picture->p->i_pitch;
    if( x1 > x2 || y1 > y2 )
        return;

    if (fill == STYLE_FILLED) {
        if(x1 == 0 && x2 + 1 == r->p_picture->p->i_visible_pitch) {
            for (int x=0; x<pitch * (y2 - y1 + 1); x+=4)
                SetPixelColor(&p[pitch * y1 + x], color);
        } else {
            for (int y = y1; y <= y2; y++)
            {
                for (int x=0; x<x2 - x1 + 1; x+=4)
                    SetPixelColor(&p[x1 + pitch * y + x], color);
            }
        }
    } else {
        DrawRect(r, STYLE_FILLED, color, x1, y1, x1, y2);
        DrawRect(r, STYLE_FILLED, color, x2, y1, x2, y2);
        DrawRect(r, STYLE_FILLED, color, x1, y1, x2, y1);
        DrawRect(r, STYLE_FILLED, color, x1, y2, x2, y2);
    }
}

/**
 * Draws a triangle at the given position in the region.
 * It may be filled (fill == STYLE_FILLED) or empty (fill == STYLE_EMPTY).
 */
static void DrawTriangle(subpicture_region_t *r, int fill, const uint8_t color[4],
                         int x1, int y1, int x2, int y2)
{
    uint8_t *p    = r->p_picture->p->p_pixels;
    int     pitch = r->p_picture->p->i_pitch;
    const int mid = y1 + (y2 - y1) / 2;
    const bool b_swap = (x1 > x2);

    for (int y = y1; y <= mid; y++) {
        const int h = y - y1;
        if (fill == STYLE_FILLED) {
            const int w = b_swap ? __MAX(x1 - h, x2) : __MIN(x1 + h, x2);
            DrawRect(r, STYLE_FILLED, color,
                     (b_swap) ? w : x1, y, (b_swap) ? x1 : w, y);
            DrawRect(r, STYLE_FILLED, color,
                     (b_swap) ? w : x1, y2 - h, (b_swap) ? x1 : w, y2 - h);
        } else {
            SetPixelColor(&p[x1 +                     pitch * 4 * y       ], color);
            SetPixelColor(&p[x1 + (b_swap ? -h : h) + pitch * 4 * y       ], color);
            SetPixelColor(&p[x1 +                     pitch * 4 * (y2 - h)], color);
            SetPixelColor(&p[x1 + (b_swap ? -h : h) + pitch * 4 * (y2 - h)], color);
        }
    }
}

/**
 * Create a region with a white transparent picture.
 */
static subpicture_region_t *OSDRegion(int x, int y, int width, int height, video_palette_t *palette)
{
    if( width == 0 || height == 0 )
        return NULL;

    SET_PALETTE_COLOR(COL_WHITE,       0xffffff, STYLE_ALPHA_OPAQUE)
    SET_PALETTE_COLOR(COL_TRANSPARENT, 0xffffff, STYLE_ALPHA_TRANSPARENT)
    SET_PALETTE_COLOR(COL_FILL,        RGB_FILL, 0xA0)
    SET_PALETTE_COLOR(COL_FILL_SHADE,  RGB_FILL, 0x25)
    palette->i_entries = 4;

    video_format_t fmt;
    video_format_Init(&fmt, VLC_CODEC_RGBA);
    fmt.i_width          =
    fmt.i_visible_width  = width;
    fmt.i_height         =
    fmt.i_visible_height = height;
    fmt.i_sar_num        = 1;
    fmt.i_sar_den        = 1;

    subpicture_region_t *r = subpicture_region_New(&fmt);
    if (!r)
        return NULL;
    r->i_x = x;
    r->i_y = y;

    return r;
}

/**
 * Create the region for an OSD slider.
 * Types are: OSD_HOR_SLIDER and OSD_VERT_SLIDER.
 */
#define SLIDER_MARGIN_BASE 0.10
static subpicture_region_t *OSDSlider(int type, int position,
                                      const video_format_t *fmt)
{
    const int size = __MAX(fmt->i_visible_width, fmt->i_visible_height);
    const int margin = size * SLIDER_MARGIN_BASE;
    const int marginbottom = margin * 0.2;
    const int marginright = margin * 0.5;
    uint8_t i_padding = __MIN(1, size * 0.25); /* small sizes */

    int x, y;
    int width, height;
    if (type == OSD_HOR_SLIDER) {
        width  = __MAX(fmt->i_visible_width - 2 * margin, 1);
        height = __MAX(fmt->i_visible_height * 0.01,      1);
        x      = __MIN(fmt->i_x_offset + margin, fmt->i_visible_width - width);
        y      = __MAX(fmt->i_y_offset + fmt->i_visible_height - marginbottom, 0);
    } else {
        width  = __MAX(fmt->i_visible_width * 0.010,       1);
        height = __MAX(fmt->i_visible_height - 2 * margin, 1);
        x      = __MAX(fmt->i_x_offset + fmt->i_visible_width - marginright, 0);
        y      = __MIN(fmt->i_y_offset + margin, fmt->i_visible_height - height);
    }

    if( (width < 1 + 2 * i_padding) || (height < 1 + 2 * i_padding) )
        return NULL;

    video_palette_t palette;
    subpicture_region_t *r = OSDRegion(x, y, width, height, &palette);
    if( !r)
        return NULL;

    int pos_x = i_padding;
    int pos_y, pos_xend;
    int pos_yend = height - 1 - i_padding;

    if (type == OSD_HOR_SLIDER ) {
        pos_y = i_padding;
        pos_xend = pos_x + (width - 2 * i_padding) * position / 100;
    } else {
        pos_y = height - (height - 2 * i_padding) * position / 100;
        pos_xend = width - 1 - i_padding;
    }

    /* one full fill is faster than drawing outline */
    DrawRect(r, STYLE_FILLED, palette.palette[COL_FILL_SHADE], 0, 0, width - 1, height - 1);
    DrawRect(r, STYLE_FILLED, palette.palette[COL_FILL], pos_x, pos_y, pos_xend, pos_yend);

    return r;
}

/**
 * Create the region for an OSD slider.
 * Types are: OSD_PLAY_ICON, OSD_PAUSE_ICON, OSD_SPEAKER_ICON, OSD_MUTE_ICON
 */
static subpicture_region_t *OSDIcon(int type, const video_format_t *fmt)
{
    const float size_ratio   = 0.05f;
    const float margin_ratio = 0.07f;

    const int size   = __MAX(fmt->i_visible_width, fmt->i_visible_height);
    const int width  = size * size_ratio;
    const int height = size * size_ratio;
    const int x      = fmt->i_x_offset + fmt->i_visible_width - margin_ratio * size - width;
    const int y      = fmt->i_y_offset                        + margin_ratio * size;

    if( width < 1 || height < 1 )
        return NULL;

    video_palette_t palette;
    subpicture_region_t *r = OSDRegion(__MAX(x, 0),
                                       __MIN(y, (int)fmt->i_visible_height - height),
                                       width, height, &palette);
    if (!r)
        return NULL;

    DrawRect(r, STYLE_FILLED, palette.palette[COL_TRANSPARENT], 0, 0, width - 1, height - 1);

    if (type == OSD_PAUSE_ICON) {
        int bar_width = width / 3;
        DrawRect(r, STYLE_FILLED, palette.palette[COL_WHITE], 0, 0, bar_width - 1, height -1);
        DrawRect(r, STYLE_FILLED, palette.palette[COL_WHITE], width - bar_width, 0, width - 1, height - 1);
    } else if (type == OSD_PLAY_ICON) {
        int mid   = height >> 1;
        int delta = (width - mid) >> 1;
        int y2    = ((height - 1) >> 1) * 2;
        DrawTriangle(r, STYLE_FILLED, palette.palette[COL_WHITE], delta, 0, width - delta, y2);
    } else {
        int mid   = height >> 1;
        int delta = (width - mid) >> 1;
        int y2    = ((height - 1) >> 1) * 2;
        DrawRect(r, STYLE_FILLED, palette.palette[COL_WHITE], delta, mid / 2, width - delta, height - 1 - mid / 2);
        DrawTriangle(r, STYLE_FILLED, palette.palette[COL_WHITE], width - delta, 0, delta, y2);
        if (type == OSD_MUTE_ICON) {
            for(int y1 = 0; y1 <= height -1; y1++)
                DrawRect(r, STYLE_FILLED, palette.palette[COL_FILL], y1, y1, __MIN(y1 + delta, width - 1), y1);
        }
    }
    return r;
}

typedef struct {
    int type;
    int position;
} osdwidget_spu_updater_sys_t;

static int OSDWidgetValidate(subpicture_t *subpic,
                           bool has_src_changed, const video_format_t *fmt_src,
                           bool has_dst_changed, const video_format_t *fmt_dst,
                           vlc_tick_t ts)
{
    VLC_UNUSED(subpic); VLC_UNUSED(ts);
    VLC_UNUSED(fmt_src); VLC_UNUSED(has_src_changed);
    VLC_UNUSED(fmt_dst);

    if (!has_dst_changed)
        return VLC_SUCCESS;
    return VLC_EGENERIC;
}

static void OSDWidgetUpdate(subpicture_t *subpic,
                          const video_format_t *fmt_src,
                          const video_format_t *fmt_dst,
                          vlc_tick_t ts)
{
    osdwidget_spu_updater_sys_t *sys = subpic->updater.p_sys;
    VLC_UNUSED(fmt_src); VLC_UNUSED(ts);

    video_format_t fmt = *fmt_dst;
    fmt.i_width         = fmt.i_width         * fmt.i_sar_num / fmt.i_sar_den;
    fmt.i_visible_width = fmt.i_visible_width * fmt.i_sar_num / fmt.i_sar_den;
    fmt.i_x_offset      = fmt.i_x_offset      * fmt.i_sar_num / fmt.i_sar_den;
    fmt.i_sar_num       = 1;
    fmt.i_sar_den       = 1;

    subpic->i_original_picture_width  = fmt.i_visible_width;
    subpic->i_original_picture_height = fmt.i_visible_height;
    if (sys->type == OSD_HOR_SLIDER || sys->type == OSD_VERT_SLIDER)
        subpic->p_region = OSDSlider(sys->type, sys->position, &fmt);
    else
        subpic->p_region = OSDIcon(sys->type, &fmt);
}

static void OSDWidgetDestroy(subpicture_t *subpic)
{
    free(subpic->updater.p_sys);
}

static void OSDWidget(vout_thread_t *vout, int channel, int type, int position)
{
    if (!var_InheritBool(vout, "osd"))
        return;
    if (type == OSD_HOR_SLIDER || type == OSD_VERT_SLIDER)
        position = VLC_CLIP(position, 0, 100);

    osdwidget_spu_updater_sys_t *sys = malloc(sizeof(*sys));
    if (!sys)
        return;
    sys->type     = type;
    sys->position = position;

    subpicture_updater_t updater = {
        .pf_validate = OSDWidgetValidate,
        .pf_update   = OSDWidgetUpdate,
        .pf_destroy  = OSDWidgetDestroy,
        .p_sys       = sys,
    };
    subpicture_t *subpic = subpicture_New(&updater);
    if (!subpic) {
        free(sys);
        return;
    }

    subpic->i_channel  = channel;
    subpic->i_start    = vlc_tick_now();
    subpic->i_stop     = subpic->i_start + VLC_TICK_FROM_MS(1200);
    subpic->b_ephemer  = true;
    subpic->b_absolute = true;
    subpic->b_fade     = true;

    vout_PutSubpicture(vout, subpic);
}

void vout_OSDSlider(vout_thread_t *vout, int channel, int position, short type)
{
    OSDWidget(vout, channel, type, position);
}

void vout_OSDIcon(vout_thread_t *vout, int channel, short type )
{
    OSDWidget(vout, channel, type, 0);
}

