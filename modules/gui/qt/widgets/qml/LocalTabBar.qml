/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11

import "qrc:///style/"

NavigableRow {
    id: row

    property var currentView
    signal clicked(int index)

    height: VLCStyle.localToolbar_height
    focus: true

    delegate: BannerTabButton {
        text: model.displayText
        selected: model.name === row.currentView.name
        showCurrentIndicator: false
        height: VLCStyle.localToolbar_height
        onClicked: row.clicked(index)
    }
}
