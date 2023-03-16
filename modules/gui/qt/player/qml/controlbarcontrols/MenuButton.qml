// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"


Widgets.IconControlButton {
    id: menuBtn

    signal requestLockUnlockAutoHide(bool lock)

    iconText: VLCIcons.ellipsis
    text: I18n.qtr("Menu")

    onClicked: contextMenu.popup(this.mapToGlobal(0, 0))

    QmlGlobalMenu {
        id: contextMenu

        ctx: MainCtx

        onAboutToShow: menuBtn.requestLockUnlockAutoHide(true)
        onAboutToHide: menuBtn.requestLockUnlockAutoHide(false)
    }
}
