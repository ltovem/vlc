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

import QtQuick
import QtQuick.Templates as T
import QtQuick.Layouts

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

T.Control {
    id: delegate

    // Properties

    property var rowModel
    property var sortModel

    property bool selected: false

    readonly property int _index: index

    property int _modifiersOnLastPress: Qt.NoModifier

    readonly property bool dragActive: dragHandler.active

    property var dragItem

    property bool acceptDrop: false

    signal contextMenuButtonClicked(Item menuParent, var menuModel, point globalMousePos)
    signal rightClick(Item menuParent, var menuModel, point globalMousePos)
    signal itemDoubleClicked(var index, var model)

    signal selectAndFocus(int modifiers, int focusReason)

    signal dropEntered(var drag, bool before)
    signal dropUpdatePosition(var drag, bool before)
    signal dropExited(var drag, bool before)
    signal dropEvent(var drag, var drop, bool before)

    property Component defaultDelegate: Widgets.TextAutoScroller {
        id: defaultDelId
        property var rowModel: parent.rowModel
        property var colModel: parent.colModel
        readonly property ColorContext colorContext: parent.colorContext
        readonly property bool selected: parent.selected

        label: text
        forceScroll: parent.currentlyFocused
        width: parent.width
        clip: scrolling

        Widgets.ListLabel {
            id: text

            anchors.verticalCenter: parent.verticalCenter
            text: defaultDelId.rowModel
                    ? (defaultDelId.rowModel[defaultDelId.colModel.criteria] || "")
                    : ""

            color: defaultDelId.selected
                ? defaultDelId.colorContext.fg.highlight
                : defaultDelId.colorContext.fg.primary
        }
    }
    // Settings

    hoverEnabled: true

    ListView.delayRemove: dragActive

    Component.onCompleted: {
        Keys.menuPressed.connect(contextButton.clicked)
    }

    // Childs

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Item

        focused: delegate.visualFocus
        hovered: delegate.hovered
    }

    DragHandler {
        id: dragHandler

        target: null

        onActiveChanged: {
            if (dragItem) {
                if (active) {
                    dragItem.Drag.active = true
                } else {
                    dragItem.Drag.drop()
                }
            }
        }
    }

    TapHandler {
        acceptedDevices: PointerDevice.AllDevices & ~(PointerDevice.TouchScreen)

        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onSingleTapped: (point, button) => {
            initialAction()

            if (button === Qt.RightButton)
                delegate.rightClick(delegate, delegate.rowModel, parent.mapToGlobal(point.position.x, point.position.y))
        }

        onDoubleTapped: (point, button) => {
            if (button === Qt.LeftButton)
                delegate.itemDoubleClicked(delegate._index, delegate.rowModel)
        }

        Component.onCompleted: {
            canceled.connect(initialAction)
        }

        function initialAction() {
            if ((point.pressedButtons === Qt.LeftButton) || !delegate.selected) {
                delegate.selectAndFocus(point.modifiers, Qt.MouseFocusReason)
            }
        }
    }

    TapHandler {
        acceptedDevices: PointerDevice.TouchScreen

        onTapped: (eventPoint, button) => {
            delegate.selectAndFocus(Qt.NoModifier, Qt.MouseFocusReason)
            delegate.itemDoubleClicked(delegate._index, delegate.rowModel)
        }

        onLongPressed: {
            delegate.rightClick(delegate, delegate.rowModel, parent.mapToGlobal(point.position.x, point.position.y))
        }
    }

    background: AnimatedBackground {
        animationDuration: VLCStyle.duration_short
        enabled: theme.initialized
        color: delegate.selected ? theme.bg.highlight : theme.bg.primary
        border.color: visualFocus ? theme.visualFocus : "transparent"
    }

    contentItem: Row {
        leftPadding: VLCStyle.margin_xxxsmall
        rightPadding: VLCStyle.margin_xxxsmall

        spacing: VLCStyle.column_spacing

        Repeater {
            model: delegate.sortModel

            Loader{
                property var rowModel: delegate.rowModel

                property var colModel: modelData.model

                readonly property int index: delegate._index

                readonly property bool currentlyFocused: delegate.activeFocus

                readonly property bool selected: delegate.selected

                readonly property bool containsMouse: delegate.hovered

                readonly property ColorContext colorContext: theme

                width: (modelData.size) ? VLCStyle.colWidth(modelData.size) : 0

                height: parent.height

                sourceComponent: colModel.colDelegate || delegate.defaultDelegate
            }
        }

        Item {
            width: VLCStyle.icon_normal

            height: parent.height

            Widgets.IconToolButton {
                id: contextButton

                anchors.left: parent.left

                // NOTE: We want the contextButton to be contained inside the trailing
                //       column_spacing.
                anchors.leftMargin: -width

                anchors.verticalCenter: parent.verticalCenter

                text: VLCIcons.ellipsis

                font.pixelSize: VLCStyle.icon_normal

                description: qsTr("Menu")

                visible: delegate.hovered

                onClicked: {
                    if (!delegate.selected)
                        delegate.selectAndFocus(Qt.NoModifier, Qt.MouseFocusReason)

                    const pos = contextButton.mapToGlobal(VLCStyle.margin_xsmall, contextButton.height / 2 + VLCStyle.fontHeight_normal)
                    delegate.contextMenuButtonClicked(this, delegate.rowModel, pos)
                }

                activeFocusOnTab: false
            }
        }
    }

    DropArea {
        enabled: delegate.acceptDrop

        anchors.fill: parent

        function isBefore(drag) {
            return drag.y < height/2
        }

        onEntered: (drag) => { delegate.dropEntered(drag, isBefore(drag)) }

        onPositionChanged: (drag) => { delegate.dropUpdatePosition(drag, isBefore(drag)) }

        onExited:delegate.dropExited(drag, isBefore(drag))

        onDropped: (drop) => { delegate.dropEvent(drag, drop, isBefore(drag)) }

    }
}
