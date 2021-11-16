/*****************************************************************************
 * Copyright (C) 2024 VLC authors and VideoLAN
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
import QtQuick 2.12

import org.videolan.vlc 0.1

import "qrc:///style/"

OverlapNotifier {
    id: root

    property alias active: rect.visible
    property alias parentItem: rect.parent

    property alias anchors: rect.anchors

    property alias width: rect.width
    property alias x: rect.x
    property alias y: rect.y
    property alias height: rect.height

    property alias color: rect.color
    property alias opacity: rect.opacity
    property alias border: rect.border
    property alias radius: rect.radius

    property alias z: rect.z

    property point beginPos
    property point endPos

    readonly property rect geometry: Qt.rect(Math.min(endPos.x, beginPos.x),
                                             Math.min(endPos.y, beginPos.y),
                                             Math.abs(endPos.x - beginPos.x),
                                             Math.abs(endPos.y - beginPos.y))

    maskItem: Rectangle {
        id: rect

        parent: containerItem
        visible: false

        z: 99

        x: geometry.x
        y: geometry.y
        width: geometry.width
        height: geometry.height

        color: "#88ADD8E6"
        border.width: VLCStyle.rubberBand_border
        border.color: Qt.darker(color)
        radius: VLCStyle.rubberBand_radius
    }

    onMaskItemChanged: {
        console.assert(maskItem === rect)
    }
}
