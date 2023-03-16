// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Controls 2.4

import org.videolan.vlc 0.1

import "qrc:///style/"

TextField {
    id: control

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.TextField

        focused: control.activeFocus
        hovered: control.hovered
        enabled: control.enabled
    }

    selectedTextColor : theme.fg.highlight
    selectionColor : theme.bg.highlight
    color : theme.fg.primary
    font.pixelSize: VLCStyle.fontSize_normal

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        border.width: control.enabled ? VLCStyle.dp(2, VLCStyle.scale) : 0
        color: theme.bg.primary
        border.color: theme.border
    }

}
