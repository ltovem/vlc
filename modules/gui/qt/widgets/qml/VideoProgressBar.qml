/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import org.videolan.vlc 0.1
import "qrc:///style/"

//we want the progress bar to match the radius of the of the video thumbnail
//so we generarte two rectangles with the right radius and we clip the part we
//want to hide
Item {
    id: progressBar

    implicitHeight: VLCStyle.dp(4, VLCStyle.scale)

    clip :true

    property real value: 0
    property int radius: implicitHeight

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Slider
    }

    Rectangle {

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: Math.max(progressBar.radius * 2, //to have at least the proper radius applied
                         parent.height + radius) //the top radius section should be always clipped

        //FIXME do we want it always white or do we want it to change with the theme
        color: "white"
        radius: progressBar.radius

        //use clipping again to delimit the viewed part as we want the accent section to follow the given as well
        Item {
            clip: true

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: progressBar.value * parent.width

            Rectangle {
                width: progressBar.width

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left

                color: theme.fg.primary
                radius: progressBar.radius
            }
        }
    }
}
