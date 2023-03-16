// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"


Widgets.IconControlButton {
    id: nextBtn
    iconText: VLCIcons.next
    enabled: mainPlaylistController.hasNext
    onClicked: mainPlaylistController.next()
    text: I18n.qtr("Next")
}
