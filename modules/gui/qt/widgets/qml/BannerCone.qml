/*****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Image {
    id: root

    property var button: MainCtx.csdButtonModel.systemMenuButton
    /* required */ property color color

    sourceSize.width: VLCStyle.icon_normal
    sourceSize.height: VLCStyle.icon_normal
    source: SVGColorImage.colorize("qrc:///misc/cone.svg")
                    .accent(root.color)
                    .uri()

    focus: false

    Loader {
        anchors.fill: root
        enabled: MainCtx.clientSideDecoration && root.button

        sourceComponent: MouseArea {
            onClicked: { root.button.click() }
            onDoubleClicked: { root.button.doubleClick() }

            Connections {
                // don't target MouseArea for position since we
                // need position updates of cone, inresepect of BannerSources
                // to correctly track cone's global position
                target: root

                // handles VLCStyle.scale changes
                onXChanged: Qt.callLater(root.updateRect)
                onYChanged: Qt.callLater(root.updateRect)
                onWidthChanged: Qt.callLater(root.updateRect)
                onHeightChanged: Qt.callLater(root.updateRect)
            }

            Connections {
                target: VLCStyle

                // handle window resize
                onAppWidthChanged: Qt.callLater(root.updateRect)
                onAppHeightChanged: Qt.callLater(root.updateRect)
            }
        }
    }

    function updateRect() {
        var rect = root.mapToItem(null, 0, 0, width, height)

        if (button)
            button.rect = rect
    }
}
