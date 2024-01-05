/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Prince Gupta <guptaprince8832@gmail.com>
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

import QtQuick 2.12

import org.videolan.vlc 0.1
import org.videolan.compat 0.1

Image {
    id: root

    property bool acceptPartialSourceSize: true

    property size targetSourceSize: Qt.size(width * MainCtx.screen.devicePixelRatio,
                                            height * MainCtx.screen.devicePixelRatio)

    property string targetSource

    property bool disableSmoothWhenIntegerUpscaling: false

    // TODO: Remove this Qt >= 5.14 (Binding.restoreMode == Binding.RestoreBindingOrValue)
    // Only required for the Binding to restore the value back
    readonly property bool _smooth: true
    smooth: _smooth

    onSourceSizeChanged: {
        if (targetSourceSize === sourceSize)
            sourceBinding.allow = true
        else if (status === Image.Null)
            sourceBinding.allow = false
    }

    BindingCompat on smooth {
        when: root.disableSmoothWhenIntegerUpscaling &&
              !((root.paintedWidth % root.implicitWidth) || (root.paintedHeight % root.implicitHeight))
        value: false
    }

    BindingCompat on sourceSize {
        // Delay source size to prevent multiple assignments.
        delayed: true
        when: root.acceptPartialSourceSize || (root.targetSourceSize.width > 0 && root.targetSourceSize.height > 0)
        value: root.targetSourceSize
    }

    BindingCompat on source {
        // source should be empty until sourceSize is finalized
        id: sourceBinding

        when: allow && root.targetSource.length > 0
        value: root.targetSource

        property bool allow: false
    }
}
