/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Widgets.ImageToolButton {
    id: control

    text: I18n.qtr("A to B")

    sourceSize.width: VLCStyle.icon_toolbar
    sourceSize.height: VLCStyle.icon_toolbar

    checked: Player.ABloopState !== Player.ABLOOP_STATE_NONE
    onClicked: Player.toggleABloopState()

    //imageSource: "qrc:///icons/atob.svg"
    imageSource: {
        switch(Player.ABloopState) {
        case Player.ABLOOP_STATE_A:
            return control._colorize(
                control.colorContext.accent,
                control.colorContext.fg.primary
            )
        case Player.ABLOOP_STATE_B:
            return control._colorize(
                control.colorContext.accent,
                control.colorContext.accent
            )
        case Player.ABLOOP_STATE_NONE:
        default:
            return control._colorize(
                control.colorContext.fg.primary,
                control.colorContext.fg.primary
            )
        }
    }

    function _colorize(a, b) {
        return SVGColorImage.colorize("qrc:///icons/atob.svg")
            .color1(control.colorContext.fg.primary)
            .any({
                "#AAAAAA": a,
                "#BBBBBB": b,
            }).uri()
    }
}
