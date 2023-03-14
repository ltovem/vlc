/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"


Widgets.ComboBoxExt {
    id: combo
    property bool paintOnly: false

    signal requestLockUnlockAutoHide(bool lock)

    width: VLCStyle.combobox_width_normal
    height: VLCStyle.combobox_height_normal
    textRole: "display"
    model: Player.aspectRatio
    currentIndex: -1
    onCurrentIndexChanged: model.toggleIndex(currentIndex)
    Accessible.name: I18n.qtr("Aspect ratio")

    Connections {
        target: combo.popup
        onOpened: combo.requestLockUnlockAutoHide(true)
        onClosed: combo.requestLockUnlockAutoHide(false)
    }
}
