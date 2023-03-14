/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

// original code from the Chromium project

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GTK_NAV_BUTTON_PROVIDER_GTK_H_
#define UI_GTK_NAV_BUTTON_PROVIDER_GTK_H_

#include "gtk_compat.h"

#include "../qtthemeprovider.hpp"

#include <map>

namespace gtk {

class NavButtonProviderGtk {
public:
    NavButtonProviderGtk();
    ~NavButtonProviderGtk();

    // views::NavButtonProvider:
    void RedrawImages(int top_area_height, bool maximized, bool active);
    VLCPicturePtr GetImage(vlc_qt_theme_csd_button_type type,
                            vlc_qt_theme_csd_button_state state) const;
    MyInset GetNavButtonMargin(
        vlc_qt_theme_csd_button_type type) const;
    MyInset GetTopAreaSpacing() const;
    int GetInterNavButtonSpacing() const;

private:
    std::map<vlc_qt_theme_csd_button_type,
             std::map<vlc_qt_theme_csd_button_state, VLCPicturePtr>>
        button_images_;
    std::map<vlc_qt_theme_csd_button_type, MyInset>
        button_margins_;
    MyInset top_area_spacing_;
    int inter_button_spacing_;
};

}  // namespace gtk

#endif  // UI_GTK_NAV_BUTTON_PROVIDER_GTK_H_
