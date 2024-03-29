/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
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
import QtQuick
import QtQuick.Controls

import org.videolan.vlc 0.1
import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Button{
    id: control
    hoverEnabled: true
    property real size: VLCStyle.icon_normal
    property color backgroundColor: theme.bg.primary

    //Accessible
    Accessible.onPressAction: control.clicked()

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.View

        enabled: control.enabled
        focused: control.visualFocus
        hovered: control.hovered
    }

    contentItem: Text {
        text: control.text
        font: control.font
        color: theme.fg.primary
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        Accessible.ignored: true
    }

    background: Rectangle {
        width: control.size
        height: control.size
        opacity: control.hovered ? 1 : 0.5
        color: control.backgroundColor
        radius: control.size/2
    }
}
