/*****************************************************************************
 * Copyright (C) 2024 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#include "kwindowsystem_module.hpp"

#include <vlc_plugin.h>

#include <KWindowEffects>

static bool isEffectAvailable(const int effect)
{
    return KWindowEffects::isEffectAvailable(static_cast<KWindowEffects::Effect>(effect));
}

static void enableBlurBehind(QWindow* const window, const bool enable = true, const QRegion& region = QRegion())
{
    KWindowEffects::enableBlurBehind(window, enable, region);
}

static int Open(vlc_object_t* const p_this)
{
    assert(p_this);
    const auto obj = reinterpret_cast<KWindowSystemModule*>(p_this);

    obj->enableBlurBehind = enableBlurBehind;
    obj->isEffectAvailable = isEffectAvailable;

    return VLC_SUCCESS;
}

vlc_module_begin()
    add_shortcut("QtKWindowSystem")
    set_description("Enables Qt GUI and KWindowSystem interaction.")
    set_capability("qtkwindowsystem", 0)
    set_callback(Open)
vlc_module_end()
