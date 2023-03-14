/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Widgets.IconControlButton {
    id: root

    signal requestLockUnlockAutoHide(bool lock)

    iconText: VLCIcons.renderer
    text: I18n.qtr("Renderer")

    // NOTE: We want to pop the menu above the button.
    onClicked: menu.popup(this.mapToGlobal(0, 0), true)

    QmlRendererMenu {
        id: menu

        ctx: MainCtx

        onAboutToShow: root.requestLockUnlockAutoHide(true)
        onAboutToHide: root.requestLockUnlockAutoHide(false)
    }
}
