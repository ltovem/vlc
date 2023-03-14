/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * Authors: Prince Gupta <guptaprince8832@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11

import "qrc:///style/"

Rectangle {
    id: root

    //---------------------------------------------------------------------------------------------
    // Settings
    //---------------------------------------------------------------------------------------------

    property bool active: activeFocus

    // background of this component changes, set it in binding, the changes will be animated
    property color backgroundColor: "transparent"

    // `foregroundColor` property is not used in this component but is
    // provided as a convenience as it gets animated with color property
    property color foregroundColor

    property color activeBorderColor

    property int animationDuration: VLCStyle.duration_long

    property bool animationRunning: borderAnimation.running || bgAnimation.running

    property bool animate: true

    //---------------------------------------------------------------------------------------------
    // Implementation
    //---------------------------------------------------------------------------------------------

    color: backgroundColor

    border.color: root.active
                  ? root.activeBorderColor
                  : VLCStyle.setColorAlpha(root.activeBorderColor, 0)

    border.width: root.active ? VLCStyle.focus_border : 0

    //---------------------------------------------------------------------------------------------
    // Animations
    //---------------------------------------------------------------------------------------------

    Behavior on border.color {
        enabled: root.animate

        ColorAnimation {
            id: borderAnimation

            duration: root.animationDuration
        }
    }

    Behavior on color {
        enabled: root.animate
        ColorAnimation {
            id: bgAnimation

            duration: root.animationDuration
        }
    }

    Behavior on foregroundColor {
        enabled: root.animate
        ColorAnimation {
            duration: root.animationDuration
        }
    }
}
