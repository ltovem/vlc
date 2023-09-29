/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
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

import "qrc:///style/"

ToolTipExt {
    id: pointingTooltip

    margins: 0
    padding: VLCStyle.margin_xxsmall

    readonly property real _x: pos.x - (width / 2)
    readonly property real _y: pos.y - (height / 2)
    /* required */ property point pos
    property bool vertical: true

    StateGroup {
        states: [
            State {
                when : vertical
                PropertyChanges {
                    target: pointingTooltip
                    x: _x
                    y: pos.y - (implicitHeight + arrowArea.implicitHeight + VLCStyle.dp(7.5))
                }

            },
            State {
                when : !vertical
                PropertyChanges {
                    target: pointingTooltip
                    x: pos.x - (implicitWidth + arrowArea.implicitWidth + VLCStyle.dp(7.5))
                    y: _y
                }
            }
        ]
    }


    background: Rectangle {
        border.color: pointingTooltip.colorContext.border
        color: pointingTooltip.colorContext.bg.primary
        radius: VLCStyle.dp(6, VLCStyle.scale)

        Item {
            id: arrowArea

            z: 1

            clip: true

            states: [
                State {
                    when : vertical
                    PropertyChanges {
                        target: arrowArea

                        anchors.topMargin:  -(parent.border.width)
                        anchors.leftMargin:  0

                        implicitHeight:  arrow.implicitHeight * Math.sqrt(2) / 2
                        implicitWidth:  0
                    }
                    AnchorChanges {
                        target: arrowArea

                        anchors.left:  parent.left
                        anchors.right:  parent.right
                        anchors.top:  parent.bottom
                        anchors.bottom:  undefined
                    }

                    PropertyChanges {
                        target: arrow

                        anchors.horizontalCenterOffset:  _x - pointingTooltip.x
                    }
                    AnchorChanges {
                        target: arrow

                        anchors.horizontalCenter: arrowArea.horizontalCenter
                        anchors.verticalCenter: arrowArea.top
                    }
                },
                State {
                    when : !vertical
                    PropertyChanges {
                        target: arrowArea

                        anchors.left: parent.right
                        anchors.right: undefined
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom

                        anchors.topMargin: 0
                        anchors.leftMargin: -(parent.border.width)

                        implicitHeight: 0
                        implicitWidth: arrow.implicitWidth * Math.sqrt(2) / 2
                    }
                    PropertyChanges {
                        target: arrow

                        anchors.horizontalCenter:  arrowArea.left
                        anchors.horizontalCenterOffset:  0
                        anchors.verticalCenter:  arrowArea.verticalCenter
                    }
                }
            ]


            Rectangle {
                id: arrow

                implicitWidth: VLCStyle.dp(10, VLCStyle.scale)
                implicitHeight: VLCStyle.dp(10, VLCStyle.scale)

                rotation: 45

                color: pointingTooltip.colorContext.bg.primary
                border.color: pointingTooltip.colorContext.border
            }
        }
    }
}
