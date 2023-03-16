// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************
 * Authors: Prince Gupta <guptaprince8832@gmail.com>
 *****************************************************************************/
pragma Singleton
import QtQuick 2.11

import org.videolan.vlc 0.1
import org.videolan.compat 0.1

Item {
    id: root

    property real uiTransluency: (enabled && MainCtx.intfMainWindow.active) ? 1 : 0

    enabled: MainCtx.hasAcrylicSurface

    Behavior on uiTransluency {
        NumberAnimation {
            duration: VLCStyle.duration_long
            easing.type: Easing.InOutSine
        }
    }

    BindingCompat {
        when: root.enabled
        target: MainCtx
        property: "acrylicActive"
        value: root.uiTransluency != 0
    }
}
