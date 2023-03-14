/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Controls 2.4

import org.videolan.vlc 0.1

import "qrc:///player/"
import "qrc:///widgets/" as Widgets
import "qrc:///style/"


Control {
    id: highResolutionTimeWidget

    property bool paintOnly: false

    padding: VLCStyle.focus_border

    Keys.priority: Keys.AfterItem
    Keys.onPressed: Navigation.defaultKeyAction(event)

    function _adjustSMPTETimer(add) {
        if (typeof toolbarEditor !== "undefined") // FIXME: Can't use paintOnly because it is set later
            return

        if (add === true)
            Player.requestAddSMPTETimer()
        else if (add === false)
            Player.requestRemoveSMPTETimer()
    }

    Component.onCompleted: {
        _adjustSMPTETimer(true)
    }

    Component.onDestruction: {
        _adjustSMPTETimer(false)
    }

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.ToolButton

        focused: highResolutionTimeWidget.visualFocus
    }

    background: Widgets.AnimatedBackground {
        active: visualFocus
        animate: theme.initialized
        activeBorderColor: theme.visualFocus
    }

    contentItem: Item {
        implicitHeight: smpteTimecodeMetrics.height
        implicitWidth: smpteTimecodeMetrics.width

        Widgets.MenuLabel {
            id: label
            anchors.fill: parent

            text: paintOnly ? "00:00:00:00" : Player.highResolutionTime
            color: theme.fg.primary

            horizontalAlignment: Text.AlignHCenter
        }

        TextMetrics {
            id: smpteTimecodeMetrics
            font: label.font
            text: "-00:00:00:00-"
        }
    }
}
