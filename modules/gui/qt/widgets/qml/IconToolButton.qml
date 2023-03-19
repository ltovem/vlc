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

    // Properties

    property bool paintOnly: false

    property int size: VLCStyle.icon_normal

    property string iconText: ""

    property color color: (control.checked) ? theme.accent : theme.fg.primary

    property color backgroundColor: theme.bg.primary

    // Settings

    padding: 0

    enabled: !paintOnly

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    // Keys

    Keys.priority: Keys.AfterItem

    Keys.onPressed: Navigation.defaultKeyAction(event)

    // Childs

    T.ToolTip.text: control.text
    T.ToolTip.delay: VLCStyle.delayToolTipAppear

    readonly property ColorContext colorContext : ColorContext {
        id: theme
        colorSet: ColorContext.ToolButton

        enabled: control.paintOnly || control.enabled
        focused: control.visualFocus
        hovered: control.hovered
        pressed: control.down
    }

    background: AnimatedBackground {
        implicitWidth: size
        implicitHeight: size

        animate: theme.initialized

        active: control.visualFocus

        backgroundColor: control.backgroundColor
        foregroundColor: control.color

        activeBorderColor: theme.visualFocus
    }

    contentItem: T.Label {
        anchors.centerIn: parent

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        text: iconText

        color: control.background.foregroundColor

        font.pixelSize: control.size
        font.family: VLCIcons.fontFamily
        font.underline: control.font.underline

        Accessible.ignored: true
    }
}
