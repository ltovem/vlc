// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets

ComboBox {
    id: control

    font.pixelSize: VLCStyle.fontSize_large
    leftPadding: 5

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.ComboBox

        focused: control.activeFocus
        enabled: control.enabled
        hovered: control.hovered
        pressed: control.pressed
    }

    property color color: theme.fg.primary
    property color bgColor: theme.bg.primary
    property color borderColor: theme.border

    Keys.priority: Keys.AfterItem
    Keys.onPressed: Navigation.defaultKeyAction(event)

    delegate: ItemDelegate {
        width: control.width
        leftPadding: control.leftPadding
        background: Item {}
        contentItem: Text {
            text: control.textRole ? (Array.isArray(control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
            color: control.color
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: control.highlightedIndex === index
    }

    indicator: Canvas {
        id: canvas
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 10
        height: 6
        contextType: "2d"

        Connections {
            target: control
            onPressedChanged: canvas.requestPaint()
        }

        onPaint: {
            if (context === null)
                return

            context.reset();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width / 2, height);
            context.closePath();
            context.fillStyle = control.activeFocus ? theme.accent : control.color;
            context.fill();
        }
    }

    contentItem: Text {
        leftPadding: 5
        rightPadding: control.indicator.width + control.spacing

        text: control.displayText
        font: control.font
        color: control.color
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: control.width
        implicitHeight: control.height
        color: control.bgColor
        border.color: control.borderColor
        border.width: control.activeFocus ? 2 : 1
        radius: 2
    }

    popup: Popup {
        y: control.height - 1

        // NOTE: This Popup should be on top of other Popup(s) most of the time.
        z: 100

        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            highlight: Widgets.AnimatedBackground {
                active: visualFocus
                animate: theme.initialized
                activeBorderColor: theme.visualFocus
                backgroundColor: theme.bg.secondary
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            color: control.bgColor
            border.color: control.borderColor
            radius: 2
        }
    }
}
