/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 * Copyright (C) 2017 The Qt Company Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Templates 2.4 as T

import "qrc:///style/"
import "qrc:///util/Helpers.js" as Helpers

T.ScrollBar {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    padding: VLCStyle.margin_xxxsmall
    visible: control.policy !== T.ScrollBar.AlwaysOff

    property VLCColors colors: (parent && Helpers.isValidInstanceOf(parent.colors, VLCColors)) ? parent.colors
                                                                                               : VLCStyle.colors

    contentItem: Rectangle {
        implicitWidth: control.interactive ? VLCStyle.scrollbarSize : VLCStyle.margin_xxxsmall
        implicitHeight: control.interactive ? VLCStyle.scrollbarSize : VLCStyle.margin_xxxsmall

        radius: width / 2
        color: control.pressed ? control.colors.scrollBarPressedColor : control.colors.scrollBarColor
        opacity: 0.0

        states: State {
            name: "active"
            when: control.policy === T.ScrollBar.AlwaysOn || (control.active && control.size < 1.0)
            PropertyChanges { target: control.contentItem; opacity: 0.75 }
        }

        transitions: Transition {
            from: "active"
            SequentialAnimation {
                PauseAnimation { duration: VLCStyle.ms500 }
                NumberAnimation { target: control.contentItem; duration: VLCStyle.duration_normal; property: "opacity"; to: 0.0 }
            }
        }
    }
}
