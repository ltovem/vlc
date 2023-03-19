// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4

import "qrc:///style/"

ToolTipExt {
    id: pointingTooltip

    margins: 0
    padding: VLCStyle.margin_xxsmall

    x: _x
    y: pos.y - (implicitHeight + arrowArea.implicitHeight + VLCStyle.dp(7.5))

    readonly property real _x: pos.x - (width / 2)
    property point pos

    background: Rectangle {
        border.color: pointingTooltip.colorContext.border
        color: pointingTooltip.colorContext.bg.primary
        radius: VLCStyle.dp(6, VLCStyle.scale)

        Item {
            id: arrowArea

            z: 1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.bottom
            anchors.topMargin: -(parent.border.width)

            implicitHeight: arrow.implicitHeight * Math.sqrt(2) / 2

            clip: true

            Rectangle {
                id: arrow

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: _x - pointingTooltip.x
                anchors.verticalCenter: parent.top

                implicitWidth: VLCStyle.dp(10, VLCStyle.scale)
                implicitHeight: VLCStyle.dp(10, VLCStyle.scale)

                rotation: 45

                color: pointingTooltip.colorContext.bg.primary
                border.color: pointingTooltip.colorContext.border
            }
        }
    }
}
