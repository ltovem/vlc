/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Layouts 1.11

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
                    value = Player.audioDelayMS

                    update = true
                }

                onValueChanged: {
                    if (update === false)
                        return

                    Player.audioDelayMS = value
                }

                Connections {
                    target: Player

                    onAudioDelayChanged: {
                        spinBox.update = false

                        spinBox.value = Player.audioDelayMS

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
