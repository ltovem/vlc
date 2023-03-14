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
    id: snapshotBtn
    enabled: Player.isPlaying
    iconText: VLCIcons.snapshot
    onClicked: Player.snapshot()
    text: I18n.qtr("Snapshot")
}
