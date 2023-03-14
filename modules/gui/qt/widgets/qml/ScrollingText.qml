/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4

import "qrc:///style/"

Item {
    id: root

    readonly property alias scrolling: scrollAnimation.running

    // `label`: label to scroll, don't add horizontal anchors on it
    property Text label: undefined
    property bool forceScroll: false
    property alias hoverScroll: hoverArea.enabled


    readonly property real requiredTextWidth: label.implicitWidth
    readonly property bool _needsToScroll: (label.width < requiredTextWidth)

    ToolTip.delay: VLCStyle.delayToolTipAppear
    ToolTip.visible: scrolling && hoverArea.containsMouse
    ToolTip.text: label.text

    onLabelChanged: {
        label.width = Qt.binding(function () { return Math.min(label.implicitWidth, root.width) })

        label.elide = Qt.binding(function () {
            return root.scrolling ? Text.ElideNone : Text.ElideRight
        })
    }

    MouseArea {
        id: hoverArea

        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        cursorShape: undefined
        hoverEnabled: true
    }

    SequentialAnimation {
        id: scrollAnimation

        running: (root.forceScroll || hoverArea.containsMouse) && root._needsToScroll
        loops: Animation.Infinite

        onStopped: {
            label.x = 0
        }

        PauseAnimation {
            duration: VLCStyle.duration_veryLong
        }

        SmoothedAnimation {
            target: label
            property: "x"
            from: 0
            to: label.width - root.requiredTextWidth

            maximumEasingTime: 0
            velocity: 20
        }

        PauseAnimation {
            duration: VLCStyle.duration_veryLong
        }

        PropertyAction {
            target: label
            property: "x"
            value: 0
        }
    }
}

