
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Templates 2.4 as T
import org.videolan.vlc 0.1

import "qrc:///style/"

T.ItemDelegate {
    id: control

    // Settings

    leftPadding: VLCStyle.margin_xlarge
    rightPadding: VLCStyle.margin_xsmall

    checkable: true

    font.pixelSize: VLCStyle.fontSize_large

    // Childs

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Item

        focused: control.activeFocus
        hovered: control.hovered
        pressed: control.pressed
        enabled: control.enabled
    }

    background: AnimatedBackground {
        active: visualFocus

        animate: theme.initialized
        backgroundColor: control.checked ? theme.bg.highlight : theme.bg.primary
        activeBorderColor: theme.visualFocus
    }

    contentItem: Item { // don't use a row, it will move text when control is unchecked
        IconLabel {
            id: checkIcon

            height: parent.height

            verticalAlignment: Text.AlignVCenter

            visible: control.checked

            text: VLCIcons.check

            color: theme.fg.primary

            font.pixelSize: VLCStyle.icon_track
        }

        MenuLabel {
            id: text

            anchors.left: checkIcon.right

            height: parent.height
            width: parent.width - checkIcon.width

            leftPadding: VLCStyle.margin_normal

            verticalAlignment: Text.AlignVCenter

            text: control.text
            font: control.font

            color: theme.fg.primary
        }
    }
}
