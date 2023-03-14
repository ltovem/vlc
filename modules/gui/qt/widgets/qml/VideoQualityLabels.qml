/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Templates 2.4 as T

import org.videolan.vlc 0.1
import "qrc:///style/"

Row {
    property alias labels: repeater.model

    spacing: VLCStyle.margin_xxsmall

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Badge
    }

    Repeater {
        id: repeater

        delegate: T.Label {
            id: label

            bottomPadding: VLCStyle.margin_xxxsmall
            topPadding: VLCStyle.margin_xxxsmall
            leftPadding: VLCStyle.margin_xxxsmall
            rightPadding: VLCStyle.margin_xxxsmall

            text: modelData
            font.pixelSize: VLCStyle.fontSize_normal

            color: theme.fg.primary

            background: Rectangle {
                anchors.fill: label
                color: theme.bg.primary
                opacity: 0.5
                radius: VLCStyle.dp(3, VLCStyle.scale)
            }

            Accessible.ignored: true
        }
    }
}
