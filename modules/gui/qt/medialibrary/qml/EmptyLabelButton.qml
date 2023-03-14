/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11
import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets

EmptyLabel {
    // Properties

    property bool _keyPressed: false

    // Aliases

    property alias button: button

    // Functions

    function onNavigate() {
        History.push(["mc", "network"])
    }

    // Keys

    Keys.priority: Keys.AfterItem

    Keys.onPressed: {
        _keyPressed = true

        Navigation.defaultKeyAction(event)
    }

    Keys.onReleased: {
        if (_keyPressed === false)
            return

        _keyPressed = false

        if (KeyHelper.matchOk(event))
            onNavigate()

        Navigation.defaultKeyReleaseAction(event)
    }

    // Children

    Widgets.ButtonExt {
        id: button

        anchors.horizontalCenter: parent.horizontalCenter

        width: Math.max(VLCStyle.dp(84, VLCStyle.scale), implicitWidth)

        focus: true

        text: I18n.qtr("Browse")
        iconTxt: VLCIcons.topbar_network

        Navigation.parentItem: root

        onClicked: onNavigate()
    }
}
