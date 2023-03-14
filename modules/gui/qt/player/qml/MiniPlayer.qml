/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Templates 2.4 as T
import QtQuick.Layouts 1.11
import QtGraphicalEffects 1.0

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

T.Pane {
    id: root

    height: 0

    implicitWidth: Math.max(background ? background.implicitWidth : 0, contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0, contentItem.implicitHeight + topPadding + bottomPadding)

    visible: false

    state: (Player.playingState === Player.PLAYING_STATE_STOPPED) ? ""
                                                                  : "expanded"

    states: State {
        name: "expanded"

        PropertyChanges {
            target: root
            visible: true
            height: implicitHeight
        }
    }

    transitions: Transition {
        from: ""; to: "expanded"
        reversible: true

        SequentialAnimation {
            // visible should change first, in order for inner layouts to calculate implicitHeight correctly
            PropertyAction { property: "visible" }
            NumberAnimation { property: "height"; easing.type: Easing.InOutSine; duration: VLCStyle.duration_long; }
        }
    }

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Window
    }

    // this MouseArea prevents mouse events to be sent below miniplayer
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.AllButtons
    }

    background: Rectangle {
        color: theme.bg.primary
    }

    contentItem: ControlBar {
        focus: true
        textPosition: ControlBar.TimeTextPosition.Hide
        sliderHeight: VLCStyle.dp(3, VLCStyle.scale)
        bookmarksHeight: VLCStyle.icon_xsmall * 0.7
        identifier: PlayerControlbarModel.Miniplayer
        Navigation.parentItem: root

        Keys.onPressed: {
            Navigation.defaultKeyAction(event)

            if (!event.accepted) {
                MainCtx.sendHotkey(event.key, event.modifiers)
            }
        }
    }
}
