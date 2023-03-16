// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Templates 2.4 as T

import "qrc:///widgets/" as Widgets
import "qrc:///style/"


Item {
    id: spacer
    enabled: false
    implicitWidth: VLCStyle.icon_toolbar
    implicitHeight: VLCStyle.icon_toolbar
    property alias spacetextExt: spacetext
    property bool paintOnly: false

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.ToolButton

        enabled: spacer.enabled || spacer.paintOnly
    }

    T.Label {
        id: spacetext
        text: VLCIcons.space
        color: theme.fg.secondary
        visible: parent.paintOnly

        anchors.centerIn: parent

        font.pixelSize: VLCStyle.icon_toolbar
        font.family: VLCIcons.fontFamily

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
