// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

import org.videolan.controls 0.1

// NOTE: This rectangle is useful to discern the item against a similar background.
// FIXME: Maybe we could refactor this to draw the background directly in the RoundImage.
Rectangle {
    id: root

    // Properties

    property real playIconSize: VLCStyle.play_cover_normal

    property real playCoverBorderWidth: VLCStyle.table_cover_border

    property bool playCoverShowPlay: true

    // Aliases

    property alias source: image.source
    property bool isImageReady: image.status == RoundImage.Ready

    property alias imageOverlay: overlay.sourceComponent

    property alias playCoverVisible: playCoverLoader.visible
    property alias playCoverOpacity: playCoverLoader.opacity

    // Signals

    signal playIconClicked(var /* MouseEvent */ mouse)

    // Settings

    height: VLCStyle.listAlbumCover_height
    width: VLCStyle.listAlbumCover_width

    // Children

    RoundImage {
        id: image

        anchors.fill: parent

        radius: root.radius
    }

    Loader {
        id: overlay

        anchors.fill: parent
    }

    Loader {
        id: playCoverLoader

        anchors.centerIn: parent

        visible: false

        active: false

        sourceComponent: Widgets.PlayCover {
            width: playIconSize

            onClicked: playIconClicked(mouse)
        }

        asynchronous: true

        // NOTE: We are lazy loading the component when this gets visible and it stays loaded.
        //       We could consider unloading it when visible goes to false.
        onVisibleChanged: if (playCoverShowPlay && visible) active = true
    }
}
