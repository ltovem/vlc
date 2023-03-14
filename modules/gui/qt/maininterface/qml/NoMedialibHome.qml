/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets

FocusScope {

    ColorContext {
        id: theme
        colorSet: ColorContext.View
    }

    Column {
        anchors.centerIn: parent

        spacing: VLCStyle.margin_small

        Widgets.IconLabel {
            text: VLCIcons.dropzone
            color: theme.fg.secondary
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: VLCStyle.dp(100, VLCStyle.scale)
        }

        Widgets.MenuLabel {
            anchors .horizontalCenter: parent.horizontalCenter
            text: I18n.qtr("Drop some content here")
            color: theme.fg.secondary
        }

        Widgets.ActionButtonPrimary {
            id: openFileButton
            text: I18n.qtr("Open File")
            focus: true
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: DialogsProvider.simpleOpenDialog()
        }
    }
}
