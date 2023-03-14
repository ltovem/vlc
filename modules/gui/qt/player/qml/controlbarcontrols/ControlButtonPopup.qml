/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Controls 2.4

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets
import "qrc:///util/Helpers.js" as Helpers

Widgets.IconControlButton {
    id: control
    // Properties

    // Private

    readonly property bool _isCurrentViewPlayer: (History.current.name === "player")

    // Aliases

    property alias popupContent: popup.contentItem

    property alias popup: popup

    // Signals

    signal requestLockUnlockAutoHide(bool lock)

    // Settings

    color: (popup.visible) ? control.colorContext.accent : control.colorContext.fg.primary

    // FIXME: We can't use upItem because a Popup is not an Item.
    Navigation.upAction: function() {
        if (popup.visible) {
            popup.forceActiveFocus(Qt.TabFocusReason)

            return
        }

        var parent = Navigation.parentItem;

        if (parent)
            parent.Navigation.defaultNavigationUp()
    }

    // Events

    onClicked: popup.open()

    // Connections

    Connections {
        target: (popup.visible) ? popup.parent : null

        onWidthChanged: _updatePosition()
        onHeightChanged: _updatePosition()
    }

    // Functions

    // Private

    // NOTE: coordinates are based on the popup parent view.
    function _updatePosition() {
        var parent = popup.parent

        var position = parent.mapFromItem(root, x, y)

        var popupX = Math.round(position.x - ((popup.width - width) / 2))

        var minimum = VLCStyle.applicationHorizontalMargin + VLCStyle.margin_xxsmall

        var maximum = parent.width - popup.width - minimum

        popup.x = Helpers.clamp(popupX, minimum, maximum)

        popup.y = position.y - popup.height - VLCStyle.margin_xxsmall
    }

    // Children

    Popup {
        id: popup

        parent: (root._isCurrentViewPlayer) ? rootPlayer : g_root

        padding: VLCStyle.margin_small

        z: 1

        focus: true

        modal: true

        // NOTE: Popup.CloseOnPressOutside doesn't work with non-model Popup on Qt < 5.15.
        closePolicy: (Popup.CloseOnPressOutside | Popup.CloseOnEscape)

        Overlay.modal: null

        // Events

        onOpened: {
            root._updatePosition()

            root.requestLockUnlockAutoHide(true)

            if (root._isCurrentViewPlayer)
                rootPlayer.applyMenu(popup)
        }

        onClosed: {
            root.requestLockUnlockAutoHide(false)

            root.forceActiveFocus()

            if (root._isCurrentViewPlayer)
                rootPlayer.applyMenu(null)
        }

        onWidthChanged: if (visible) root._updatePosition()
        onHeightChanged: if (visible) root._updatePosition()

        background: Rectangle {
            ColorContext {
                id: popupTheme
                palette: control.colorContext.palette
                colorSet: ColorContext.Window
            }

            radius: VLCStyle.dp(8, VLCStyle.scale)

            opacity: 0.85

            color: popupTheme.bg.primary
        }
    }
}
