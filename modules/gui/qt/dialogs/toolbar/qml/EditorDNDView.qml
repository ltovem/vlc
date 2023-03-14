/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQml.Models 2.11

import org.videolan.vlc 0.1
import org.videolan.compat 0.1

import "qrc:///style/"
import "qrc:///util/" as Util

ListView {
    id: playerBtnDND

    spacing: VLCStyle.margin_xsmall
    orientation: Qt.Horizontal
    clip: true

    currentIndex: -1
    highlightFollowsCurrentItem: false

    property bool containsDrag: footerItem.dropVisible

    property alias scrollBar: scrollBar

    property bool extraWidthAvailable: true

    ScrollBar.horizontal: ScrollBar {
        id: scrollBar
        policy: playerBtnDND.contentWidth > playerBtnDND.width ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
    }

    remove: Transition {
        NumberAnimation {
            property: "opacity"; from: 1.0; to: 0

            duration: VLCStyle.duration_long
        }
    }

    // FIXME: Animations are disabled because they are incompatible
    // with the delegate loader which sets extra width after the
    // control gets loaded.
    /*
    add: Transition {
        NumberAnimation {
            property: "opacity"; from: 0; to: 1.0

            duration: VLCStyle.duration_long
        }
    }

    displaced: Transition {
        NumberAnimation {
            properties: "x"

            duration: VLCStyle.duration_long
            easing.type: Easing.OutSine
        }

        NumberAnimation { property: "opacity"; to: 1.0 }
    }
    */

    function dropEvent(drag, destIndex) {
        if (drag.source.dndView === playerBtnDND) {
            // moving from same section
            playerBtnDND.model.move(drag.source.DelegateModel.itemsIndex,
                                    destIndex)
        } else if (drag.source.objectName === "buttonsList") {
            // moving from buttonsList
            playerBtnDND.model.insert(destIndex, {"id" : drag.source.mIndex})
        } else {
            // moving between sections or views
            playerBtnDND.model.insert(destIndex, {"id" : drag.source.controlId})
            drag.source.dndView.model.remove(drag.source.DelegateModel.itemsIndex)
        }
    }

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.View
    }

    MouseEventFilter {
        target: playerBtnDND
    }

    Util.FlickableScrollHandler {
        fallbackScroll: true
        enabled: true
    }

    MouseArea {
        anchors.fill: parent

        acceptedButtons: Qt.NoButton
        z: -1

        cursorShape: root.dragActive ? Qt.DragMoveCursor : Qt.ArrowCursor
    }

    footer: Item {
        anchors.verticalCenter: parent.verticalCenter

        implicitHeight: VLCStyle.icon_medium

        BindingCompat on implicitWidth {
            delayed: true
            value: Math.max(implicitHeight, playerBtnDND.width - x)
        }

        property alias dropVisible: footerDropArea.containsDrag

        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            z: 2

            implicitWidth: VLCStyle.dp(2, VLCStyle.scale)

            visible: dropVisible
            color: theme.accent
        }

        DropArea {
            id: footerDropArea

            anchors.fill: parent

            onEntered: {
                if (drag.source.dndView === playerBtnDND &&
                        drag.source.DelegateModel.itemsIndex === playerBtnDND.count - 1) {
                    drag.accepted = false
                }
            }

            onDropped: {
                var destIndex = playerBtnDND.count

                if (drag.source.dndView === playerBtnDND)
                    --destIndex

                dropEvent(drag, destIndex)
            }
        }
    }

    delegate: EditorDNDDelegate {
        anchors.verticalCenter: (!!parent) ? parent.verticalCenter : undefined

        dndView: playerBtnDND

        BindingCompat {
            when: dropArea.containsDrag
            value: true

            target: playerBtnDND
            property: "containsDrag"
        }
    }
}
