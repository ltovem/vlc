/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4

import org.videolan.vlc 0.1
import org.videolan.medialib 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Item {
    id: control

    property real barHeight
    property int yShift

    property bool controlBarHovered

    property bool bookmarkHovered
    property string bookmarkText
    property real bookmarkPosition

    implicitHeight: control.barHeight
    height: implicitHeight

    visible: false
    opacity: 0

    Widgets.PointingTooltip {
        id: timeTooltip

        //tooltip is a Popup, palette should be passed explicitly
        colorContext.palette: theme.palette

        visible: control.bookmarkHovered
        text: control.bookmarkText
        pos: Qt.point(control.bookmarkPosition, - yShift)
    }

    transitions: [
        Transition {
            to: "hidden"
            SequentialAnimation {
                NumberAnimation{
                    target: control
                    properties: "opacity"
                    to: 0
                    duration: VLCStyle.duration_short; easing.type: Easing.OutSine
                }
                PropertyAction { target: control; property: "visible"; value: false; }
            }
        },
        Transition {
            to: "visible"
            SequentialAnimation {
                PropertyAction { target: control; property: "visible"; value: true; }
                NumberAnimation{
                    target: control
                    properties: "opacity"
                    to: 1
                    duration: VLCStyle.duration_short; easing.type: Easing.InSine
                }
            }
        }
    ]

    state: controlBarHovered && bookmarksRptr.count > 0 ? "visible" : "hidden"

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Window
    }

    Repeater {
        id: bookmarksRptr
        model: MLBookmarkModel {
            id: modelBookmark

            player: Player

            ml: MediaLib
        }

        Widgets.IconToolButton {
            id: bookmarkButton

            size: control.barHeight

            x: control.width * model.position - width/2
            focusPolicy: Qt.NoFocus

            onClicked: modelBookmark.select(modelBookmark.index(index, 0))

            onHoveredChanged: {
                if(bookmarkButton.hovered){
                    control.bookmarkText = model.name
                    control.bookmarkPosition = x + width/2
                }
                control.bookmarkHovered = bookmarkButton.hovered
            }

            background: Rectangle {
                radius: parent.width * 0.5
                color: bookmarkButton.colorContext.fg.primary
            }
        }
    }
}
