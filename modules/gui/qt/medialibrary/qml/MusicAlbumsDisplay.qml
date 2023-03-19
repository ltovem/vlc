// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/

import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets

MusicAlbums {
    id: root

    onCurrentIndexChanged: {
        History.update(["mc","music", "albums", {"initialIndex": currentIndex}])
    }
}
