// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4

import org.videolan.vlc 0.1
import "qrc:///style/"

Rectangle {

    property int orientation: Qt.Vertical
    property int margin: VLCStyle.margin_xxxsmall

    readonly property ColorContext colorContext: ColorContext {
        id: theme
    }

    color: theme.accent
    width: orientation === Qt.Vertical ? VLCStyle.heightBar_xxxsmall : parent.width
    height: orientation === Qt.Horizontal ? VLCStyle.heightBar_xxxsmall : parent.height

    onOrientationChanged: {
        if (orientation == Qt.Vertical) {
            anchors.horizontalCenter = undefined
            anchors.verticalCenter = Qt.binding(function () {
                return parent.verticalCenter
            })
            anchors.left = Qt.binding(function () {
                return parent.left
            })
            anchors.right = undefined
            anchors.leftMargin = Qt.binding(function () {
                return margin
            })
            anchors.bottomMargin = 0
        } else {
            anchors.top = undefined
            anchors.bottom = Qt.binding(function () {
                return parent.bottom
            })
            anchors.horizontalCenter = Qt.binding(function () {
                return parent.horizontalCenter
            })
            anchors.verticalCenter = undefined
            anchors.leftMargin = 0
            anchors.bottomMargin = Qt.binding(function () {
                return margin
            })
        }
    }
}
