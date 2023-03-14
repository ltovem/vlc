/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

import org.videolan.vlc 0.1

Widgets.IconControlButton {
    iconText: VLCIcons.fullscreen

    onClicked: {
        if (History.current.view === "player")
            History.previous()
        else
            g_mainDisplay.showPlayer()
    }

    text: I18n.qtr("Switch Player")
}
