/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Templates 2.4 as T
import QtQuick.Layouts 1.11

import "qrc:///style/"


T.Button {
    id: control

    property color color
    property color hoverColor
    property string iconTxt: ""
    property bool showHovered: false
    property bool isThemeDark: false

    readonly property bool _paintHovered: control.hovered || showHovered

    padding: 0
    width: VLCStyle.dp(40, VLCStyle.scale)
    implicitWidth: contentItem.implicitWidth
    implicitHeight: contentItem.implicitHeight
    focusPolicy: Qt.NoFocus

    background: Rectangle {
        height: control.height
        width: control.width
        color: {
            if (control.pressed)
                return control.isThemeDark ? Qt.lighter(control.hoverColor, 1.2)
                                           : Qt.darker(control.hoverColor, 1.2)

            if (control._paintHovered)
                return control.hoverColor

            return "transparent"
        }
    }

    contentItem: Item {
        IconLabel {
            id: icon
            anchors.centerIn: parent
            text: control.iconTxt
            font.pixelSize: VLCStyle.icon_CSD
            color: control.color
        }
    }
}
