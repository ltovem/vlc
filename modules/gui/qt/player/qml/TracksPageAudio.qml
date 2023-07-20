/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
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

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets

TracksPage {
    id: root

    // Functions

    function textFromValue(value, locale) {
        return I18n.qtr("%1 ms").arg(Number(value).toLocaleString(locale, 'f', 0))
    }

    function valueFromText(text, locale) {
        return Number.fromLocaleString(locale, text.substring(0, text.length - 3))
    }

    // Children

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right

        spacing: VLCStyle.margin_xxsmall

        Widgets.SubtitleLabel {
            Layout.fillWidth: true

            text: I18n.qtr("Audio track synchronization")

            color: root.colorContext.fg.primary
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: VLCStyle.margin_large

            spacing: VLCStyle.margin_xsmall

            Accessible.role: Accessible.Grouping
            Accessible.name: I18n.qtr("Audio track delay")

            Widgets.MenuCaption {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter

                text: I18n.qtr("Audio track delay")

                color: root.colorContext.fg.primary
            }

            Widgets.SpinBoxExt {
                id: spinBox

                property bool update: false

                Layout.preferredWidth: VLCStyle.dp(128, VLCStyle.scale)

                stepSize: 50
                from: -10000
                to: 10000

                textFromValue: root.textFromValue
                valueFromText: root.valueFromText

                Navigation.parentItem: root
                Navigation.rightItem: reset

                Component.onCompleted: {
                    value = MainPlayerController.audioDelayMS

                    update = true
                }

                onValueChanged: {
                    if (update === false)
                        return

                    MainPlayerController.audioDelayMS = value
                }

                Connections {
                    target: MainPlayerController

                    onAudioDelayChanged: {
                        spinBox.update = false

                        spinBox.value = MainPlayerController.audioDelayMS

                        spinBox.update = true
                    }
                }
            }

            Widgets.ActionButtonOverlay {
                id: reset

                text: I18n.qtr("Reset")

                onClicked: spinBox.value = 0

                Navigation.parentItem: root
                Navigation.leftItem: spinBox
            }
        }
    }
}
