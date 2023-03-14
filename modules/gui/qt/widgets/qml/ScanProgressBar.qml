/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Templates 2.4 as T

import org.videolan.vlc 0.1
import org.videolan.medialib 0.1

import "qrc:///style/"

T.ProgressBar {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    rightPadding: VLCStyle.margin_large
    leftPadding: VLCStyle.margin_large
    bottomPadding: VLCStyle.margin_small
    topPadding: VLCStyle.margin_small

    from: 0
    to: 100

    value: MediaLib.parsingProgress

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Window
    }

    indeterminate: MediaLib.discoveryPending

    background: Rectangle {
        color: theme.bg.primary
    }

    contentItem: Column {

        spacing: VLCStyle.margin_small

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            implicitHeight: VLCStyle.heightBar_xxsmall
            implicitWidth: 200

            ColorContext {
                id: progressBarTheme
                colorSet: ColorContext.Slider
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                implicitHeight: VLCStyle.heightBar_xxxsmall

                color: progressBarTheme.bg.primary
            }

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter

                implicitWidth: parent.width * control.visualPosition
                implicitHeight: VLCStyle.heightBar_xxsmall

                // NOTE: We want round corners.
                radius: height

                visible: !control.indeterminate


                color: progressBarTheme.accent
            }

            Rectangle {
                property real position: 0

                anchors.verticalCenter: parent.verticalCenter

                // NOTE: Why 0.24 though ?
                implicitWidth: parent.width * 0.24
                implicitHeight: VLCStyle.heightBar_xxsmall

                x: Math.round((parent.width - width) * position)

                // NOTE: We want round corners.
                radius: height
                visible: control.indeterminate

                color: progressBarTheme.accent

                SequentialAnimation on position {
                    loops: Animation.Infinite

                    running: visible

                    NumberAnimation {
                        from: 0
                        to: 1.0

                        duration: VLCStyle.durationSliderBouncing
                        easing.type: Easing.OutBounce
                    }

                    NumberAnimation {
                        from: 1.0
                        to: 0

                        duration: VLCStyle.durationSliderBouncing
                        easing.type: Easing.OutBounce
                    }
                }
            }
        }

        SubtitleLabel {
            anchors.left: parent.left
            anchors.right: parent.right

            text: (MediaLib.discoveryPending) ? I18n.qtr("Scanning %1")
                                                .arg(MediaLib.discoveryEntryPoint)
                                              : I18n.qtr("Indexing Medias (%1%)")
                                                .arg(MediaLib.parsingProgress)

            elide: Text.ElideMiddle

            font.pixelSize: VLCStyle.fontSize_large
            font.weight: Font.Normal
            color: theme.fg.primary
        }
    }
}
