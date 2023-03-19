// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1
import org.videolan.medialib 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///util/Helpers.js" as Helpers
import "qrc:///style/"

Widgets.GridItem {
    id: root

    property alias showNewIndicator: image.visible

    property var labels: [
        model.resolution_name || "",
        model.channel || ""
    ].filter(function(a) { return a !== "" })

    function play() {
        if ( model.id !== undefined ) {
            MediaLib.addAndPlay( model.id )
            g_mainDisplay.showPlayer()
        }
    }

    image: model.thumbnail || VLCStyle.noArtVideoCover
    title: model.title || I18n.qtr("Unknown title")
    subtitle: model.duration.formatHMS() || ""
    pictureWidth: VLCStyle.gridCover_video_width
    pictureHeight: VLCStyle.gridCover_video_height
    playCoverBorderWidth: VLCStyle.gridCover_video_border
    titleMargin: VLCStyle.margin_xxsmall

    pictureOverlay: Item {
        width: root.pictureWidth
        height: root.pictureHeight

        Widgets.VideoQualityLabels {
            anchors {
                top: parent.top
                right: parent.right
                topMargin: VLCStyle.margin_xxsmall
                leftMargin: VLCStyle.margin_xxsmall
                rightMargin: VLCStyle.margin_xxsmall
            }

            labels: root.labels
        }

        Widgets.VideoProgressBar {
            id: progressBar

            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            visible: (model.progress > 0)

            radius: root.pictureRadius
            value: Helpers.clamp(model.progress !== undefined ? model.progress : 0, 0, 1)
        }
    }

    onPlayClicked: root.play()

    Image {
        id: image

        anchors.right: parent.right
        anchors.top: parent.top

        width: VLCStyle.gridItem_newIndicator
        height: width

        visible: false

        source: VLCStyle.newIndicator
    }
}
