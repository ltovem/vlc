/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"


Widgets.IconControlButton {
    id: playlistBtn
    iconText: VLCIcons.playlist
    onClicked: {
        MainCtx.playlistVisible = !MainCtx.playlistVisible
        if (MainCtx.playlistVisible && MainCtx.playlistDocked) {
            playlistWidget.gainFocus(playlistBtn)
        }
    }
    checked: MainCtx.playlistVisible

    text: I18n.qtr("Playlist")
}
