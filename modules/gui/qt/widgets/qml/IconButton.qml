// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Templates 2.4 as T

import org.videolan.vlc 0.1

import "qrc:///style/"

T.Button {
    id: control

    width: content.implicitWidth
    height: content.implicitHeight

    property color color: "white"
    font.family: VLCIcons.fontFamily

    Keys.priority: Keys.AfterItem
    Keys.onPressed: Navigation.defaultKeyAction(event)

    contentItem: Item {
        T.Label {
            id: content
            anchors.centerIn: parent
            text: control.text
            color: control.color
            font: control.font
        }
    }

    background: Item {
        implicitWidth: 10
        implicitHeight: 10
    }
}
