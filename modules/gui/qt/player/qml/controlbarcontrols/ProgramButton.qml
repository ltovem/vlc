// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *****************************************************************************/

import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Widgets.IconControlButton {
    id: root

    signal requestLockUnlockAutoHide(bool lock)

    iconText: VLCIcons.tv

    text: I18n.qtr("Programs")

    // NOTE: We want to pop the menu above the button.
    onClicked: menu.popup(this.mapToGlobal(0, 0), true)

    enabled: (paintOnly === false && Player.hasPrograms)

    QmlProgramMenu {
        id: menu

        player: Player

        onAboutToShow: root.requestLockUnlockAutoHide(true)
        onAboutToHide: root.requestLockUnlockAutoHide(false)
    }
}
