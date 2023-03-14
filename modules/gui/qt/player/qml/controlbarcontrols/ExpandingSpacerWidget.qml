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


Item {
    enabled: false

    implicitWidth: paintOnly ? VLCStyle.widthExtendedSpacer : Number.MAX_VALUE
    implicitHeight: VLCStyle.icon_toolbar

    property bool paintOnly: false
    property alias spacetextExt: spacetext

    readonly property real minimumWidth: 0

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.ButtonStandard
    }

    T.Label {
        id: spacetext
        anchors.centerIn: parent

        text: VLCIcons.space
        color: theme.fg.secondary
        visible: paintOnly

        font.pixelSize: VLCStyle.icon_toolbar
        font.family: VLCIcons.fontFamily

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
