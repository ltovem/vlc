// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *****************************************************************************
 * Authors: Prince Gupta <guptaprince8832@gmail.com>
 *****************************************************************************/

import QtQuick 2.11

import org.videolan.vlc 0.1
import org.videolan.compat 0.1

Image {
    id: root
    sourceSize: Qt.size(width * MainCtx.screen.devicePixelRatio
                    , height * MainCtx.screen.devicePixelRatio)

    property bool disableSmoothWhenIntegerUpscaling: false

    // TODO: Remove this Qt >= 5.14 (Binding.restoreMode == Binding.RestoreBindingOrValue)
    // Only required for the Binding to restore the value back
    readonly property bool _smooth: true
    smooth: _smooth

    BindingCompat on smooth {
        when: root.disableSmoothWhenIntegerUpscaling &&
              !((root.paintedWidth % root.implicitWidth) || (root.paintedHeight % root.implicitHeight))
        value: false
    }
}
