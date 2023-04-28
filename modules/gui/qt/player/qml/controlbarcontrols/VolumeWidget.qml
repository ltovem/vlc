/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
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
import "qrc:///style/"

T.Pane {
    id: root

    property bool paintOnly: false

    readonly property var _player: paintOnly ? ({ muted: false, volume: .5 }) : Player

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    readonly property real minimumWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                                                  volumeBtn.implicitWidth + leftPadding + rightPadding)
    readonly property real preferredWidth: minimumWidth + volumeSlider.preferredWidth

    contentWidth: volumeWidget.implicitWidth
    contentHeight: volumeWidget.implicitHeight

    Keys.priority: Keys.AfterItem
    Keys.onPressed: Navigation.defaultKeyAction(event)

    RowLayout {
        id: volumeWidget

        anchors.fill: parent
        spacing: 0

        Widgets.IconToolButton{
            id: volumeBtn

            focus: true
            paintOnly: root.paintOnly
            iconText:
                if( _player.muted )
                    VLCIcons.volume_muted
                else if ( _player.volume === 0 )
                    VLCIcons.volume_zero
                else if ( _player.volume < .33 )
                    VLCIcons.volume_low
                else if( _player.volume <= .66 )
                    VLCIcons.volume_medium
                else
                    VLCIcons.volume_high
            text: I18n.qtr("Mute")
            onClicked: Player.muted = !Player.muted

            Accessible.onIncreaseAction: {
                Player.muted = false
                Player.setVolumeUp()
            }

            Accessible.onDecreaseAction: {
                Player.muted = false
                Player.setVolumeDown()
            }

            Navigation.parentItem: root
            Navigation.rightItem: volumeSlider
        }

        Widgets.VolumeSlider {
            id: volumeSlider

            paintOnly: root.paintOnly

            // FIXME: use VLCStyle
            readonly property real preferredWidth: VLCStyle.dp(100, VLCStyle.scale)
            readonly property real minimumWidth: VLCStyle.dp(50, VLCStyle.scale)

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: VLCStyle.margin_xsmall

            opacity: _player.muted ? 0.5 : 1

            enabled: visible && !root.paintOnly // disables event handling depending on paintOnly
            visible: root.width > (volumeBtn.implicitWidth + minimumWidth)

            Navigation.leftItem: volumeBtn
            Navigation.parentItem: root
        }
    }
}
