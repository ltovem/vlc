/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Templates 2.4 as T

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///player/" as P

ControlButtonPopup {
    id: root

    popup.width: VLCStyle.dp(256, VLCStyle.scale)

    text: I18n.qtr("Playback Speed")

    popupContent: P.PlaybackSpeed {
        colorContext.palette: root.colorContext.palette

        Navigation.parentItem: root

        // NOTE: Mapping the right direction because the down action triggers the ComboBox.
        Navigation.rightItem: root
    }

    // Children

    T.Label {
        anchors.centerIn: parent

        font.pixelSize: VLCStyle.fontSize_normal

        text: !root.paintOnly ? I18n.qtr("%1x").arg(+Player.rate.toFixed(2))
                              : I18n.qtr("1x")

        // IconToolButton.background is a AnimatedBackground
        color: root.background.foregroundColor
    }
}
