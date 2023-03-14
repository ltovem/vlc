/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets

Widgets.BannerTabButton {
    id: root

    implicitWidth: VLCStyle.button_width_large

    signal dropEnterred()

    DropArea {
        anchors.fill: parent

        onEntered: {
            root.dropEnterred()
        }
    }
}
