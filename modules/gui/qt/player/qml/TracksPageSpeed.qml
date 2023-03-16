// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *****************************************************************************
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *****************************************************************************/

import QtQuick 2.11

import org.videolan.vlc 0.1

TracksPage {
    id: root

    // Children

    PlaybackSpeed {
        anchors.left: parent.left
        anchors.right: parent.right

        Navigation.parentItem: root
    }
}
