/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "gtk_compat.h"

namespace gtk {

MyInset GtkStyleContextGetPadding(GtkStyleContext* context)
{
    GtkBorder padding;
    gtk_style_context_get_padding(context, gtk_style_context_get_state(context), &padding);
    return MyInset(padding);
}

MyInset GtkStyleContextGetBorder(GtkStyleContext* context)
{
    GtkBorder border;
    gtk_style_context_get_border(context, gtk_style_context_get_state(context), &border);
    return MyInset(border);
}

MyInset GtkStyleContextGetMargin(GtkStyleContext* context)
{
    GtkBorder margin;
    gtk_style_context_get_margin(context, gtk_style_context_get_state(context), &margin);
    return MyInset(margin);
}

gdouble GdkRBGALightness(GdkRGBA& c1)
{
    return 0.2126 * c1.red + 0.7152 * c1.green + 0.0722 * c1.blue;
}

GdkRGBA GdkRBGABlend(GdkRGBA& c1, GdkRGBA& c2, gdouble blend)
{
    GdkRGBA out;
    out.red = c2.red   + (c1.red   - c2.red)   * blend,
    out.green = c2.green + (c1.green - c2.green) * blend,
    out.blue = c2.blue  + (c1.blue  - c2.blue)  * blend,
    out.alpha = c2.alpha + (c1.alpha - c2.alpha) * blend;
    return out;
}

};
