/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Templates 2.4 as T
import QtQuick.Layouts 1.11

import org.videolan.medialib 0.1
import org.videolan.controls 0.1
import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

T.Control {
    id: root

    // Properties

    /* required */ property MLModel mlModel

    property bool isCurrent: false

    // Aliases
    // Private

    readonly property bool _isHover: contentItem.containsMouse || root.activeFocus

    // Signals

    signal itemClicked(var mouse)

    signal itemDoubleClicked(var mouse)

    // Settings

    implicitHeight: VLCStyle.play_cover_small + (VLCStyle.margin_xsmall * 2)

    // Childs

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Item

        focused: root.activeFocus
        hovered: contentItem.containsMouse
        enabled: root.enabled
    }

    background: Widgets.AnimatedBackground {
        active: visualFocus

        animate: theme.initialized
        backgroundColor: root.isCurrent ? theme.bg.highlight : theme.bg.primary
        activeBorderColor: theme.visualFocus
    }

    contentItem: MouseArea {
        hoverEnabled: true

        drag.axis: Drag.XAndYAxis

        drag.target: Widgets.DragItem {
            indexes: [index]

            titleRole: "name"

            onRequestData: {
                setData(identifier, [model])
            }

            function getSelectedInputItem(cb) {
                return MediaLib.mlInputItem([model.id], cb)
            }
        }

        drag.onActiveChanged: {
            var dragItem = drag.target;

            if (drag.active == false)
                dragItem.Drag.drop();

            dragItem.Drag.active = drag.active;
        }

        onPositionChanged: {
            if (drag.active == false) return;

            var pos = drag.target.parent.mapFromItem(root, mouseX, mouseY);

            drag.target.x = pos.x + VLCStyle.dragDelta;
            drag.target.y = pos.y + VLCStyle.dragDelta;
        }

        onClicked: itemClicked(mouse)

        onDoubleClicked: itemDoubleClicked(mouse)

        Widgets.CurrentIndicator {
            height: parent.height - (margin * 2)

            margin: VLCStyle.dp(4, VLCStyle.scale)

            visible: isCurrent
        }

        RowLayout {
            anchors.fill: parent

            anchors.leftMargin: VLCStyle.margin_normal
            anchors.rightMargin: VLCStyle.margin_normal
            anchors.topMargin: VLCStyle.margin_xsmall
            anchors.bottomMargin: VLCStyle.margin_xsmall

            spacing: VLCStyle.margin_xsmall

            RoundImage {
                Layout.preferredWidth: VLCStyle.play_cover_small
                Layout.preferredHeight: Layout.preferredWidth

                radius: width

                source: (model.cover) ? model.cover
                                      : VLCStyle.noArtArtistSmall

                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

                Rectangle {
                    anchors.fill: parent

                    radius: VLCStyle.play_cover_small

                    color: "transparent"

                    border.width: VLCStyle.dp(1, VLCStyle.scale)

                    border.color: (isCurrent || _isHover) ? theme.accent
                                                          : theme.border
                }
            }

            Widgets.ScrollingText {
                label: artistName

                forceScroll: root.isCurrent || root._isHover
                clip: scrolling

                Layout.fillWidth: true
                Layout.fillHeight: true

                Widgets.ListLabel {
                    id: artistName

                    anchors {
                        verticalCenter: parent.verticalCenter
                    }

                    text: (model.name) ? model.name
                                       : I18n.qtr("Unknown artist")

                    color: theme.fg.primary
                }
            }

        }
    }
}
