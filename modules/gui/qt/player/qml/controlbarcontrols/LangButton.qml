// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"
import "qrc:///player/" as Player


Widgets.IconControlButton {
    id: langBtn
    iconText: VLCIcons.audiosub

    enabled: menuLoader.status === Loader.Ready
    onClicked: menuLoader.item.open()

    signal requestLockUnlockAutoHide(bool lock)

    text: I18n.qtr("Languages and tracks")

    Loader {
        id: menuLoader

        active: (typeof rootPlayer !== 'undefined') && (rootPlayer !== null)

        sourceComponent: Player.TracksMenu {
            id: menu

            parent: rootPlayer
            focus: true
            x: 0
            y: (rootPlayer.positionSliderY - height)
            z: 1

            colorContext.palette: langBtn.colorContext.palette

            onOpened: {
                langBtn.requestLockUnlockAutoHide(true)
                if (!!rootPlayer)
                    rootPlayer.applyMenu(menu)
            }

            onClosed: {
                langBtn.requestLockUnlockAutoHide(false)
                langBtn.forceActiveFocus()
                if (!!rootPlayer)
                    rootPlayer.applyMenu(null)
            }
        }
    }
}
