/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Templates 2.4 as T

import org.videolan.vlc 0.1
import "qrc:///style/"

T.ToolTip {
    id: control

    x: 0
    y: -implicitHeight - VLCStyle.margin_xxxsmall

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    margins: VLCStyle.margin_xsmall
    padding: VLCStyle.margin_xsmall

    font.pixelSize: VLCStyle.fontSize_normal

    closePolicy: T.Popup.CloseOnEscape | T.Popup.CloseOnPressOutsideParent | T.Popup.CloseOnReleaseOutsideParent

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Tooltip
    }

    contentItem: Text {
        text: control.text
        font: control.font

        color: theme.fg.primary
    }

    background: Rectangle {
        border.color: theme.border
        color: theme.bg.primary
    }
}
