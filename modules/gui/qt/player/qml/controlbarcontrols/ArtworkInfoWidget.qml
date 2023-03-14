/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"


AbstractButton {
    id: artworkInfoItem

    property bool paintOnly: false

    readonly property real minimumWidth: coverRect.implicitWidth +
                                         + (leftPadding + rightPadding)

    property bool _keyPressed: false

    padding: VLCStyle.focus_border

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

    onClicked: {
        g_mainDisplay.showPlayer()
    }

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.ToolButton
        focused: artworkInfoItem.visualFocus
        hovered: artworkInfoItem.hovered
    }

    background: Widgets.AnimatedBackground {
        active: visualFocus
        animate: theme.initialized
        activeBorderColor: theme.visualFocus
    }

    contentItem: RowLayout {
        spacing: infoColumn.visible ? VLCStyle.margin_xsmall : 0

        Rectangle {
            id: coverRect

            implicitHeight: VLCStyle.dp(60, VLCStyle.scale)
            implicitWidth: implicitHeight

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
                        && Player.artwork
                        && Player.artwork.toString())
                        Player.artwork
                    else
                        VLCStyle.noArtAlbumCover
                }

                fillMode: Image.PreserveAspectFit

                asynchronous: true

                ToolTip.visible: infoColumn.width < infoColumn.implicitWidth
                                 && (artworkInfoItem.hovered || artworkInfoItem.visualFocus)
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
                        Player.title
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
                        Player.artist
                }
                color: theme.fg.secondary
            }

            Widgets.MenuCaption {
                id: progressIndicator

                Layout.fillWidth: true
                Layout.fillHeight: true

                text: {
                    if (paintOnly)
                        " -- / -- "
                    else
                        Player.time.formatHMS() + " / " + Player.length.formatHMS()
                }
                color: theme.fg.secondary
            }
        }
    }
}
