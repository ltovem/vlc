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
import org.videolan.compat 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

AbstractButton {
    id: root

    // Properties

    property bool paintOnly: false

    Layout.minimumWidth: height

    implicitHeight: 0

    property bool _keyPressed: false

    readonly property ColorContext colorContext: ColorContext {
        id: theme

        colorSet: ColorContext.ToolButton

        focused: root.visualFocus
        hovered: root.hovered
    }

    // Settings

    text: I18n.qtr("Open player")

    padding: VLCStyle.focus_border

    Accessible.onPressAction: root.clicked()

    // Keys

    Keys.onPressed: (event) => {
        if (KeyHelper.matchOk(event)) {
            event.accepted = true

            _keyPressed = true
        } else {
            Navigation.defaultKeyAction(event)
        }
    }

    Keys.onReleased: (event) => {
        if (_keyPressed === false)
            return

        _keyPressed = false

        if (KeyHelper.matchOk(event)) {
            event.accepted = true

            History.push(["player"])
        }
    }

    // Events

    onClicked: History.push(["player"])

    // Children

    Widgets.DragItem {
        id: dragItem

        onRequestData: (_, resolve, reject) => {
            resolve([{
                "title": Player.title,
                "cover": (!!Player.artwork && Player.artwork.toString() !== "") ? Player.artwork
                                                                                : VLCStyle.noArtAlbumCover
            }])
        }

        onRequestInputItems: (indexes, data, resolve, reject) => {
            resolve([MainPlaylistController.currentItem])
        }

        indexes: [0]
    }

    DragHandler {
        target: null
        onActiveChanged: {
            if (active) {
                dragItem.Drag.active = true
            } else {
                dragItem.Drag.drop()
            }
        }
    }

    background: Widgets.AnimatedBackground {
        enabled: theme.initialized
        border.color: visualFocus ? theme.visualFocus : "transparent"
    }

    contentItem: RowLayout {
        spacing: VLCStyle.margin_xsmall

        Widgets.ScaledImage {
            id: coverImage

            Layout.fillHeight: true
            Layout.preferredWidth: root.height

            source: {
                if (!paintOnly && Player.artwork && Player.artwork.toString())
                    return Player.artwork
                else
                    return VLCStyle.noArtAlbumCover
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

            Widgets.DefaultShadow {
                anchors.centerIn: coverImage

                sourceItem: coverImage

            }
        }

        ColumnLayout {
            id: infoColumn

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 0.1 // FIXME: Qt layout bug

            spacing: 0

            Widgets.MenuLabel {
                id: titleLabel

                Layout.fillWidth: true
                Layout.fillHeight: true

                visible: text.length > 0

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

                BindingCompat on visible {
                    delayed: (MainCtx.qtVersion() < MainCtx.qtVersionCheck(5, 15, 8))
                    value: (infoColumn.height > infoColumn.implicitHeight) && (artistLabel.text.length > 0)
                }

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

                visible: text.length > 0

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
