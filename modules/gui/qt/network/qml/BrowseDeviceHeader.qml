// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Layouts 1.11

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets

FocusScope {
    id: root

    // Properties

    /* required */ property var view

    // Aliases

    property alias text: label.text

    property alias label: label
    property alias button: button

    // Signals

    signal clicked(int reason)

    // Settings

    width: view.width
    height: label.height

    Navigation.navigable: button.visible

    // Children

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.View
    }

    RowLayout {
        id: row

        anchors.fill: parent

        anchors.leftMargin: root.view.contentMargin
        anchors.rightMargin: anchors.leftMargin

        Widgets.SubtitleLabel {
            id: label

            Layout.fillWidth: true
            color: theme.fg.primary

            topPadding: VLCStyle.margin_large
            bottomPadding: VLCStyle.margin_normal
        }

        Widgets.TextToolButton {
            id: button

            Layout.preferredWidth: implicitWidth

            focus: true

            text: I18n.qtr("See All")

            font.pixelSize: VLCStyle.fontSize_large

            Navigation.parentItem: root

            onClicked: root.clicked(focusReason)
        }
    }
}
