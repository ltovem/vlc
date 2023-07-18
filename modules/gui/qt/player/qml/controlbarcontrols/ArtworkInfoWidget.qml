/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
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
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

AbstractButton {
    id: root

    // Properties

    property bool paintOnly: false

    readonly property real minimumWidth: coverRect.implicitWidth +
                                         + (leftPadding + rightPadding)

    property int maximumHeight: _preferredHeight

    readonly property int preferredWidth: minimumWidth + contentItem.spacing * 2
                                          +
                                          Math.max(titleLabel.implicitWidth,
                                                   artistLabel.implicitWidth,
                                                   progressIndicator.implicitWidth)

    readonly property ColorContext colorContext: ColorContext {
        id: theme

        colorSet: ColorContext.ToolButton

        focused: root.visualFocus
        hovered: root.hovered
    }

    property int _preferredHeight: VLCStyle.dp(60, VLCStyle.scale)

    property bool _keyPressed: false

    // Settings

    text: I18n.qtr("Open player")

    padding: VLCStyle.focus_border

    Accessible.onPressAction: root.clicked()

    // Keys

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
            event.accepted = true

            g_mainDisplay.showPlayer()
        }
    }

    // Events

    onClicked: g_mainDisplay.showPlayer()

    background: Widgets.AnimatedBackground {
        active: visualFocus
        animate: theme.initialized
        activeBorderColor: theme.visualFocus
    }

    // Children

    contentItem: RowLayout {
        spacing: infoColumn.visible ? VLCStyle.margin_xsmall : 0

        Rectangle {
            id: coverRect

            implicitWidth: implicitHeight

            implicitHeight: Math.min(root._preferredHeight, root.maximumHeight)

            color: theme.bg.primary

            Widgets.DoubleShadow {
                anchors.fill: parent

                primaryBlurRadius: VLCStyle.dp(3, VLCStyle.scale)
                primaryVerticalOffset: VLCStyle.dp(1, VLCStyle.scale)

                secondaryBlurRadius: VLCStyle.dp(14, VLCStyle.scale)
                secondaryVerticalOffset: VLCStyle.dp(6, VLCStyle.scale)
            }

            Widgets.ScaledImage {
                id: cover

                anchors.fill: parent

                source: {
                    if (!paintOnly
                        && MainPlayerController.artwork
                        && MainPlayerController.artwork.toString())
                        MainPlayerController.artwork
                    else
                        VLCStyle.noArtAlbumCover
                }

                fillMode: Image.PreserveAspectFit

                asynchronous: true

                Accessible.role: Accessible.Graphic
                Accessible.name: I18n.qtr("Cover")

                ToolTip.visible: infoColumn.width < infoColumn.implicitWidth
                                 && (root.hovered || root.visualFocus)
                ToolTip.delay: VLCStyle.delayToolTipAppear
                ToolTip.text: I18n.qtr("%1\n%2\n%3").arg(titleLabel.text)
                                                    .arg(artistLabel.text)
                                                    .arg(progressIndicator.text)
            }
        }

        ColumnLayout {
            id: infoColumn

            Layout.fillWidth: true
            Layout.preferredHeight: coverRect.height
            Layout.minimumWidth: 0.1 // FIXME: Qt layout bug

            Widgets.MenuLabel {
                id: titleLabel

                Layout.fillWidth: true
                Layout.fillHeight: true

                text: {
                    if (paintOnly)
                        I18n.qtr("Title")
                    else
                        MainPlayerController.title
                }
                color: theme.fg.primary
            }

            Widgets.MenuCaption {
                id: artistLabel

                Layout.fillWidth: true
                Layout.fillHeight: true

                text: {
                    if (paintOnly)
                        I18n.qtr("Artist")
                    else
                        MainPlayerController.artist
                }
                color: theme.fg.secondary
            }

            Widgets.MenuCaption {
                id: progressIndicator

                Layout.fillWidth: true
                Layout.fillHeight: true

                visible: (infoColumn.height >= root._preferredHeight)

                text: {
                    if (paintOnly)
                        " -- / -- "
                    else
                        MainPlayerController.time.formatHMS() + " / " + MainPlayerController.length.formatHMS()
                }
                color: theme.fg.secondary
            }
        }
    }
}
