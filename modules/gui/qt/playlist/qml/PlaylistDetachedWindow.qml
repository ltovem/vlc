// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Window 2.11

import org.videolan.vlc 0.1

import "qrc:///style/"


Window {
    visible: true

    // TODO: Qt >5.13 use transientParent
    property QtWindow parentWindow: MainCtx.intfMainWindow

    width: 350
    minimumWidth: playlistView.minimumWidth

    title: I18n.qtr("Playlist")
    color: theme.bg.primary

    Loader {
        asynchronous: true
        source: "qrc:///menus/GlobalShortcuts.qml"
    }

    Component.onCompleted: {
        if (!!parentWindow) {
            height = parentWindow.height
            minimumHeight = parentWindow.minimumHeight

            x = parentWindow.x + parentWindow.width + 10
            y = parentWindow.y
        } else {
            height = 400
            minimumHeight = 200
        }
    }

    PlaylistListView {
        id: playlistView

        useAcrylic: false
        focus: true
        anchors.fill: parent

        colorContext.palette: VLCStyle.palette

        readonly property PlaylistListView g_root: playlistView
    }
}
