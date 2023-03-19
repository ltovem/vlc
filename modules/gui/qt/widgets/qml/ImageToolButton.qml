// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Templates 2.4 as T

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets

import "qrc:///style/"

T.ToolButton {
    id: control

    property url imageSource: ""

    property bool paintOnly: false

    property size sourceSize: Qt.size(VLCStyle.icon_normal, VLCStyle.icon_normal)

    padding: 0

    enabled: !paintOnly

    implicitWidth: control.sourceSize.width + leftPadding + rightPadding
    implicitHeight: control.sourceSize.height + topPadding + bottomPadding
    baselineOffset: contentItem.y + contentItem.baselineOffset


    Keys.priority: Keys.AfterItem
    Keys.onPressed: Navigation.defaultKeyAction(event)

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.ToolButton

        enabled: control.enabled || control.paintOnly
        focused: control.visualFocus
        hovered: control.hovered
        pressed: control.down
    }

    background: AnimatedBackground {
        width: control.sourceSize.width
        height: control.sourceSize.height

        active: control.visualFocus
        animate: theme.initialized

        backgroundColor: theme.bg.primary
        foregroundColor: theme.fg.primary
        activeBorderColor: theme.visualFocus
    }

    contentItem: Image {
        anchors.centerIn: control

        source: control.imageSource

        fillMode: Image.PreserveAspectFit

        width: control.sourceSize.width
        height: control.sourceSize.height
        sourceSize: control.sourceSize

    }

}
