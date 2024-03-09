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

import "qrc:///util/Helpers.js" as Helpers

Item {
    id: root

    property alias containerItem: overlapNotifier.containerItem

    property alias enabled: mouseArea.enabled

    readonly property bool active: overlapNotifier.active

    property bool horizontalLock: false
    property bool verticalLock: false

    property int clearSelectionExclusionModifiers: (Qt.ControlModifier)
    readonly property alias activationButtons: mouseArea.acceptedButtons
    property int contextMenuButton: Qt.RightButton

    property bool enforceFocus: true

    signal clearSelection()
    signal toggleSelection(Item item)
    signal showContextMenu(point globalPos)

    // Using mouse area here is fine since rubber band selection
    // is intended to be used with a mouse.
    MouseArea {
        id: mouseArea

        anchors.fill: parent

        acceptedButtons: (Qt.LeftButton | Qt.RightButton)

        property point globalPos: mapToGlobal(mouseX, mouseY)

        preventStealing: true

        onPressed: function(mouse) {
            focus = true // Grab the focus from delegate

            if (containerItem && containerItem.parent)
                containerItem.parent.forceActiveFocus(Qt.MouseFocusReason) // Re-focus the view

            if (!(mouse.modifiers & root.clearSelectionExclusionModifiers)) {
                root.clearSelection()
            }

            mouse.accepted = true
        }

        onReleased: function(mouse) {
            if (mouse.button & Qt.RightButton) {
                root.showContextMenu(globalPos)
            }
        }
    }

    RubberBandOverlapNotifier {
        id: overlapNotifier
        active: mouseArea.pressed

        onActiveChanged: {
            if (active) {
                beginPos = getMappedMousePos()
                endPos = beginPos
            }
        }

        Component.onCompleted: {
            mouseArea.globalPosChanged.connect(updateEndPos)
            overlapToggled.connect(root.toggleSelection)
        }

        readonly property Connections _connections : Connections {
            target: containerItem

            onXChanged: Qt.callLater(overlapNotifier.updateEndPos)
            onYChanged: Qt.callLater(overlapNotifier.updateEndPos)
        }

        function getMappedMousePos() {
            return containerItem.mapFromGlobal(mouseArea.globalPos.x,
                                               mouseArea.globalPos.y)
        }

        function updateEndPos() {
            if (overlapNotifier.active) {
                endPos = getMappedMousePos()
            }
        }

        anchors {
            left: root.horizontalLock ? parentItem.left
                                      : undefined
            right: root.horizontalLock ? parentItem.right
                                       : undefined
            top: root.verticalLock ? parentItem.top
                                   : undefined
            bottom: root.verticalLock ? parentItem.bottom
                                      : undefined
        }
    }
}
