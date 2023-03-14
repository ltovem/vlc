/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * Authors: Prince Gupta <guptaprince8832@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///style/"


ViewBlockingRectangle {
    id: root

    readonly property bool usingAcrylic: visible && enabled && AcrylicController.enabled

    property color tintColor: "gray"

    property color alternativeColor: tintColor

    readonly property color _actualTintColor: VLCStyle.setColorAlpha(tintColor, 0.7)
    property real _blend: usingAcrylic ? AcrylicController.uiTransluency : 0


    color: VLCStyle.blendColors(root._actualTintColor, root.alternativeColor, root._blend)
}
