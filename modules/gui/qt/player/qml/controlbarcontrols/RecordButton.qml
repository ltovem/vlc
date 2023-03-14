/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Templates 2.4 as T

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"


Widgets.IconToolButton {
    id: control

    enabled: !paintOnly && Player.isPlaying

    color: "red" //red means recording
    text: I18n.qtr("record")

    onClicked: Player.toggleRecord()

    contentItem: T.Label {
        anchors.centerIn: parent

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        text: VLCIcons.record
        color: control.color

        ColorAnimation on color {
            from:  "transparent"
            to: control.color
            //this is an animation and not a transisition, we explicitly want a long duration
            duration: 1000
            loops: Animation.Infinite
            easing.type: Easing.InOutSine
            running: control.enabled && Player.recording

            onStopped: {
                control.contentItem.color = control.color
            }
        }

        font.pixelSize: control.size
        font.family: VLCIcons.fontFamily
        font.underline: control.font.underline

        Accessible.ignored: true
    }
}
