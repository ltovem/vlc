/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4

import org.videolan.vlc 0.1
import "qrc:///style/"

Button{
    id: control
    hoverEnabled: true
    property real size: VLCStyle.icon_normal

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.View

        enabled: control.enabled
        focused: control.visualFocus
        hovered: control.hovered
    }

    contentItem: Text {
        text: control.text
        font: control.font
        color: theme.fg.primary
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        implicitWidth: control.size
        implicitHeight: control.size
        opacity: control.hovered ? 1 : 0.5
        color: theme.bg.primary
        radius: VLCStyle.icon_normal/2
    }
}
