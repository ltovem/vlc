/*****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Templates 2.12 as T
import QtGraphicalEffects 1.12

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///util/Helpers.js" as Helpers
import "qrc:///style/"

Widgets.Slider {
    id: root

    property bool paintOnly: false

    property bool _inhibitPlayerVolumeUpdate: false

    // FIXME: Currently we are not updating the ShiftModifier status while dragging. This
    //        could be fixed with a custom Slider based on a MouseArea.
    property bool _shiftPressed: false

    property real _clamp: 0.01

    property bool _keyPressed: false

    readonly property color sliderColor: {
        var theme = root.colorContext
        return (root.position > _fullvolpos) ? theme.fg.negative : theme.fg.primary
    }

    color: sliderColor

    readonly property int _maxvol: MainCtx.maxVolume
    readonly property real _fullvolpos: 100 / _maxvol
    readonly property real _maxvolpos: _maxvol / 100

    from: 0
    to: _maxvolpos

    toolTipTextProvider: function () {
        return Math.round(Player.volume * 100) + "%"
    }

    Accessible.name: I18n.qtr("Volume")

    Keys.onPressed: {
        if (KeyHelper.matchOk(event)) {
            event.accepted = true

            _keyPressed = true
        } else {
            Navigation.defaultKeyAction(event)
        }
    }

    Keys.onReleased: {
        if (_keyPressed === false)
            return

        _keyPressed = false

        if (KeyHelper.matchOk(event)) {
            Player.muted = !Player.muted
        }
    }

    function _syncVolumeWithPlayer() {
        root._inhibitPlayerVolumeUpdate = true
        root.value = _player.volume
        root._inhibitPlayerVolumeUpdate = false
    }

    function _adjustPlayerVolume() {
        Player.muted = false

        var value = root.value

        // NOTE: We are clamping the value to make it easier to restore the default volume.
        if (_shiftPressed === false) {
            if (Math.abs(value - 1.0) < _clamp)
                value = 1.0
            else
                _clamp = 0.01
        }

        Player.volume = value

        root.value = value
    }

    Component.onCompleted: {
        paintOnlyChanged.connect(_syncVolumeWithPlayer)
        root._syncVolumeWithPlayer()
    }

    Connections {
        target: Player
        enabled: !paintOnly

        onVolumeChanged: root._syncVolumeWithPlayer()
    }

    Keys.onUpPressed: {
        Player.muted = false
        Player.setVolumeUp()
    }

    Keys.onDownPressed: {
        Player.muted = false
        Player.setVolumeDown()
    }

    Keys.priority: Keys.BeforeItem

    onValueChanged: {
        if (paintOnly)
            return

        if (!root._inhibitPlayerVolumeUpdate) {
            Qt.callLater(root._adjustPlayerVolume)
        }
    }

    Rectangle{
        id: tickmark

        parent: root.background

        x : parent.width * _fullvolpos
        width: VLCStyle.dp(1, VLCStyle.scale)
        height: parent.height
        radius: VLCStyle.dp(2, VLCStyle.scale)

        // NOTE: This shouldn't be visible when the volume stops before a 100.
        visible: (_maxvol > 100)

        // FIXME: tick mark is not visible when it's over slider
        color: sliderColor

        // NOTE: This is a helper to select the default volume when clicking on the
        //       tickmark. We apply a higher clamp value to achieve that behavior on
        //       the Slider.
        MouseArea {
            anchors.fill: parent

            anchors.margins: -(VLCStyle.dp(4, VLCStyle.scale))

            onPressed: {
                mouse.accepted = false

                if (mouse.modifiers === Qt.ShiftModifier)
                    return

                root._clamp = 0.1
            }
        }
    }

    MouseArea {
        id: sliderMouseArea

        anchors.fill: parent

        acceptedButtons: (Qt.LeftButton | Qt.RightButton)

        onPressed: {
            if (paintOnly) {
                mouse.accepted = true
                return
            }

            root._shiftPressed = (mouse.modifiers === Qt.ShiftModifier)

            if (mouse.button === Qt.LeftButton) {
                mouse.accepted = false

                return
            }

            adjustVolume(mouse)
        }

        onPositionChanged: if (mouse.buttons & Qt.RightButton) adjustVolume(mouse)

        onWheel: {
            let delta = 0, fineControl = false

            if ((Math.abs(wheel.pixelDelta.x) % 120 > 0) || (Math.abs(wheel.pixelDelta.y) % 120 > 0)) {
                if (Math.abs(wheel.pixelDelta.x) > Math.abs(wheel.pixelDelta.y))
                    delta = wheel.pixelDelta.x
                else
                    delta = wheel.pixelDelta.y
                fineControl = true
            }
            else if (wheel.angleDelta.x)
                delta = wheel.angleDelta.x
            else if (wheel.angleDelta.y)
                delta = wheel.angleDelta.y

            if (delta === 0)
                return

            if (wheel.inverted)
                delta = -delta

            if (fineControl)
                root.value += 0.001 * delta
            else
                Helpers.applyVolume(Player, delta)

            wheel.accepted = true
        }

        function adjustVolume(mouse) {
            mouse.accepted = true

            const pos = mouse.x * _maxvolpos / width

            if (pos < 0.25)
                root.value = 0
            else if (pos < 0.75)
                root.value = 0.5
            else if (pos < 1.125)
                root.value = 1
            else
                root.value = 1.25
        }
    }
}
