/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
