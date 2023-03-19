// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Templates 2.4 as T
import QtQuick.Layouts 1.11
import QtQml.Models 2.11

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets
import "qrc:///playlist/" as PL


Control {
    id: root

    enum TimeTextPosition {
        Hide,
        AboveSlider,
        LeftRightSlider
    }

    readonly property alias sliderY: row2.y
    property int textPosition: ControlBar.TimeTextPosition.AboveSlider
    property alias identifier: playerControlLayout.identifier
    property alias sliderHeight: trackPositionSlider.barHeight
    property real bookmarksHeight: VLCStyle.icon_xsmall * 0.9

    signal requestLockUnlockAutoHide(bool lock)

    function showChapterMarks() {
        trackPositionSlider.showChapterMarks()
    }

    Keys.priority: Keys.AfterItem
    Keys.onPressed: root.Navigation.defaultKeyAction(event)
    Navigation.cancelAction: function() { History.previous() }

    onActiveFocusChanged: if (activeFocus) trackPositionSlider.forceActiveFocus(focusReason)

    Component.onCompleted: {
        // if initially textPosition = Hide, then _onTextPositionChanged isn't called
        if (textPosition === ControlBar.TimeTextPosition.Hide)
            _layout()
    }

    onTextPositionChanged: _layout()

    function _layout() {
        switch (textPosition) {
            case ControlBar.TimeTextPosition.Hide:
                row1.children = []
                row2.children = [trackPositionSlider]
                mediaTime.visible = false
                mediaRemainingTime.visible = false
                spacer.visible = false
                row2.Layout.leftMargin = 0
                row2.Layout.rightMargin = 0
                mediaTime.font.pixelSize = Qt.binding(function() { return VLCStyle.fontSize_normal })
                mediaRemainingTime.font.pixelSize = Qt.binding(function() { return VLCStyle.fontSize_normal })
                break;

            case ControlBar.TimeTextPosition.AboveSlider:
                row1.children = [mediaTime, spacer, mediaRemainingTime]
                row2.children = [trackPositionSlider]
                mediaTime.visible = true
                mediaRemainingTime.visible = Qt.binding(function() { return !playlistVisibility.isPlaylistVisible })
                spacer.visible = true
                row2.Layout.leftMargin = 0
                row2.Layout.rightMargin = 0
                mediaTime.font.pixelSize = Qt.binding(function() { return VLCStyle.fontSize_normal })
                mediaRemainingTime.font.pixelSize = Qt.binding(function() { return VLCStyle.fontSize_normal })
                break;

            case ControlBar.TimeTextPosition.LeftRightSlider:
                row1.children = []
                row2.children = [mediaTime, trackPositionSlider, mediaRemainingTime]
                mediaTime.visible = true
                mediaRemainingTime.visible = true
                spacer.visible = false
                row2.Layout.leftMargin = VLCStyle.margin_xsmall
                row2.Layout.rightMargin = VLCStyle.margin_xsmall
                mediaTime.font.pixelSize = Qt.binding(function() { return VLCStyle.fontSize_small })
                mediaRemainingTime.font.pixelSize = Qt.binding(function() { return VLCStyle.fontSize_small })
                trackPositionSlider.Layout.alignment = Qt.AlignVCenter
                break;

            default:
                console.assert(false, "invalid text position")
        }

        row1.visible = row1.children.length > 0
        row2.visible = row2.children.length > 0
    }

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Window
    }

    contentItem: ColumnLayout {
        spacing: VLCStyle.margin_xsmall
        z: 1

        RowLayout {
            id: row1

            spacing: 0
            Layout.fillWidth: true
            Layout.leftMargin: VLCStyle.margin_normal
            Layout.rightMargin: VLCStyle.margin_normal
        }

        RowLayout {
            id: row2

            spacing: VLCStyle.margin_xsmall
            Layout.fillWidth: true
        }

        PlayerControlLayout {
            id: playerControlLayout

            Layout.fillWidth: true
            Layout.leftMargin: VLCStyle.margin_large
            Layout.rightMargin: VLCStyle.margin_large
            Layout.bottomMargin: VLCStyle.margin_xsmall

            Navigation.upItem: trackPositionSlider.enabled ? trackPositionSlider : root.Navigation.upItem

            onRequestLockUnlockAutoHide: root.requestLockUnlockAutoHide(lock)
        }
    }

    T.Label {
        id: mediaTime

        text: Player.time.formatHMS()
        color: theme.fg.primary
    }

    T.Label {
        id: mediaRemainingTime

        text: (MainCtx.showRemainingTime && Player.remainingTime.valid())
              ? "-" + Player.remainingTime.formatHMS()
              : Player.length.formatHMS()
        color: theme.fg.primary

        MouseArea {
            anchors.fill: parent
            onClicked: MainCtx.showRemainingTime = !MainCtx.showRemainingTime
        }
    }

    Item {
        id: spacer

        Layout.fillWidth: true
    }

    SliderBar {
        id: trackPositionSlider

        barHeight: VLCStyle.heightBar_xxsmall
        Layout.fillWidth: true
        enabled: Player.playingState === Player.PLAYING_STATE_PLAYING || Player.playingState === Player.PLAYING_STATE_PAUSED

        Navigation.parentItem: root
        Navigation.downItem: playerControlLayout

        activeFocusOnTab: true

        Keys.onPressed: {
            Navigation.defaultKeyAction(event)
        }
    }

    Loader {
        id: bookmarksLoader

        active: MainCtx.mediaLibraryAvailable
        source: "qrc:/player/Bookmarks.qml"

        x: root.leftPadding + trackPositionSlider.x + row2.Layout.leftMargin
        y: row2.y + row2.height + VLCStyle.margin_xxsmall
        width: trackPositionSlider.width

        onLoaded: {
           item.barHeight = Qt.binding(function() { return bookmarksHeight })
           item.controlBarHovered = Qt.binding(function() { return root.hovered })
           item.yShift = Qt.binding(function() { return row2.height + VLCStyle.margin_xxsmall })
        }
    }
}
