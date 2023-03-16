// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *****************************************************************************
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Layouts 1.11

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets

RowLayout {
    id: root

    // Properties

    default property alias content: content.data

    property int preferredWidth: VLCStyle.dp(512, VLCStyle.scale)

    // Settings

    spacing: 0

    focus: true

    Navigation.leftItem: button

    // Signals

    signal backRequested

    // Children

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Window
    }

    Item {
        Layout.preferredWidth: VLCStyle.dp(72, VLCStyle.scale)
        Layout.fillHeight: true

        Layout.topMargin: VLCStyle.margin_large

        Layout.alignment: Qt.AlignLeft | Qt.AlignTop

        Widgets.IconTrackButton {
            id: button

            anchors.horizontalCenter: parent.horizontalCenter

            iconText: VLCIcons.back

            Navigation.parentItem: root
            Navigation.rightItem: content

            onClicked: root.backRequested()
        }
    }

    Rectangle {
        Layout.preferredWidth: VLCStyle.margin_xxxsmall
        Layout.fillHeight: true

        color: theme.border
    }

    FocusScope {
        id: content

        Layout.fillWidth: true
        Layout.fillHeight: true

        Layout.margins: VLCStyle.margin_large
    }
}
