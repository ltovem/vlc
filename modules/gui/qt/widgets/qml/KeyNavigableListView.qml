/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
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
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

import QtGraphicalEffects 1.12

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///util/" as Util
import "qrc:///util/Helpers.js" as Helpers

FadingEdgeListView {
    id: root

    // Properties

    readonly property int scrollBarWidth: scroll_id.visible ? scroll_id.width : 0

    property bool keyNavigationWraps: false

    // NOTE: Fading is disabled by default, 'enableBeginningFade' and 'enableEndFade' take
    // precedence over 'enableFade'.
    property bool enableFade: false

    // Private

    property bool _keyPressed: false

    // Aliases

    //forward view properties
    property alias listScrollBar: scroll_id

    property alias dragAutoScrollDragItem: dragAutoScrollHandler.dragItem
    property alias dragAutoScrollMargin: dragAutoScrollHandler.margin
    property alias dragAutoScrolling: dragAutoScrollHandler.scrolling

    // Signals

    signal selectionUpdated(int keyModifiers, int oldIndex, int newIndex)

    signal selectAll()

    signal actionAtIndex(int index)

    signal deselectAll()

    signal showContextMenu(point globalPos)

    // Private

    property int _currentFocusReason: Qt.OtherFocusReason

    // Settings

    focus: true

    activeFocusOnTab: true

    backgroundColor: theme.bg.primary

    //key navigation is reimplemented for item selection
    keyNavigationEnabled: false

    ScrollBar.vertical: ScrollBar { id: scroll_id }
    ScrollBar.horizontal: ScrollBar { }

    highlightMoveDuration: 300 //ms
    highlightMoveVelocity: 1000 //px/s

    section.property: ""
    section.criteria: ViewSection.FullString
    section.delegate: sectionHeading

    enableBeginningFade: (enableFade && dragAutoScrollHandler.scrollingDirection
                                        !==
                                        Util.ViewDragAutoScrollHandler.Backward)

    enableEndFade: (enableFade && dragAutoScrollHandler.scrollingDirection
                                  !==
                                  Util.ViewDragAutoScrollHandler.Forward)

    Accessible.role: Accessible.List

    // Events

    Component.onCompleted: {
        if (typeof root.reuseItems === "boolean") {
            // Qt 5.15 feature, on by default here:
            root.reuseItems = true
        }
    }

    // NOTE: We always want a valid 'currentIndex' by default.
    onCountChanged: if (count && currentIndex === -1) currentIndex = 0

    onCurrentItemChanged: {
        if (_currentFocusReason === Qt.OtherFocusReason)
            return;

        // NOTE: We make sure the view has active focus before enforcing it on the item.
        if (root.activeFocus && currentItem)
            Helpers.enforceFocus(currentItem, _currentFocusReason);

        _currentFocusReason = Qt.OtherFocusReason;
    }

    // Functions

    // NOTE: This function is useful to set the currentItem without losing the visual focus.
    function setCurrentItem(index) {
        if (currentIndex === index)
            return

        let reason

        if (currentItem)
            reason = currentItem.focusReason
        else
            reason = _currentFocusReason

        currentIndex = index

        if (reason !== Qt.OtherFocusReason) {
            if (currentItem)
                Helpers.enforceFocus(currentItem, reason)
            else
                setCurrentItemFocus(reason)
        }
    }

    function setCurrentItemFocus(reason) {
        if (!model || model.count === 0) {
            // NOTE: By default we want the focus on the flickable.
            root.forceActiveFocus(reason);

            // NOTE: Saving the focus reason for later.
            _currentFocusReason = reason;

            return;
        }

        if (currentIndex === -1)
            currentIndex = 0;

        positionViewAtIndex(currentIndex, ItemView.Contain);

        Helpers.enforceFocus(currentItem, reason);
    }

    function nextPage() {
        root.contentX += (Math.min(root.width, (root.contentWidth - root.width - root.contentX)))
    }

    function prevPage() {
        root.contentX -= Math.min(root.width,root.contentX - root.originX)
    }

    Keys.onPressed: {
        let newIndex = -1

        if (orientation === ListView.Vertical)
        {
            if ( KeyHelper.matchDown(event) ) {
                if (currentIndex !== count - 1 )
                    newIndex = currentIndex + 1
                else if ( root.keyNavigationWraps )
                    newIndex = 0
            } else if ( KeyHelper.matchPageDown(event) ) {
                newIndex = Math.min(count - 1, currentIndex + 10)
            } else if ( KeyHelper.matchUp(event) ) {
                if ( currentIndex !== 0 )
                    newIndex = currentIndex - 1
                else if ( root.keyNavigationWraps )
                    newIndex = count - 1
            } else if ( KeyHelper.matchPageUp(event) ) {
                newIndex = Math.max(0, currentIndex - 10)
            }
        }else{
            if ( KeyHelper.matchRight(event) ) {
                if (currentIndex !== count - 1 )
                    newIndex = currentIndex + 1
                else if ( root.keyNavigationWraps )
                    newIndex = 0
            }
            else if ( KeyHelper.matchPageDown(event) ) {
                newIndex = Math.min(count - 1, currentIndex + 10)
            } else if ( KeyHelper.matchLeft(event) ) {
                if ( currentIndex !== 0 )
                    newIndex = currentIndex - 1
                else if ( root.keyNavigationWraps )
                    newIndex = count - 1
            } else if ( KeyHelper.matchPageUp(event) ) {
                newIndex = Math.max(0, currentIndex - 10)
            }
        }

        // these events are matched on release
        if (event.matches(StandardKey.SelectAll) || KeyHelper.matchOk(event)) {
            event.accepted = true

            _keyPressed = true
        }

        const oldIndex = currentIndex
        if (newIndex >= 0 && newIndex < count && newIndex !== oldIndex) {
            event.accepted = true;

            currentIndex = newIndex;

            selectionUpdated(event.modifiers, oldIndex, newIndex);

            // NOTE: If we skip this call the item might end up under the header.
            positionViewAtIndex(currentIndex, ItemView.Contain);

            // NOTE: We make sure we have the proper visual focus on components.
            if (oldIndex < currentIndex)
                Helpers.enforceFocus(currentItem, Qt.TabFocusReason);
            else
                Helpers.enforceFocus(currentItem, Qt.BacktabFocusReason);
        }

        if (!event.accepted) {
            root.Navigation.defaultKeyAction(event)
        }
    }

    Keys.onReleased: {
        if (_keyPressed === false)
            return

        _keyPressed = false

        if (event.matches(StandardKey.SelectAll)) {
            event.accepted = true
            selectAll()
        } else if (KeyHelper.matchOk(event)) { //enter/return/space
            event.accepted = true
            actionAtIndex(currentIndex)
        }
    }

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.View
    }


    Component {
        id: sectionHeading

        Column {
            width: parent.width

            Text {
                text: section
                font.pixelSize: VLCStyle.fontSize_xlarge
                color: theme.accent
            }

            Rectangle {
                width: parent.width
                height: 1
                color: theme.border
            }
        }
    }

    Util.ViewDragAutoScrollHandler {
        id: dragAutoScrollHandler

        view: root
    }

    Util.FlickableScrollHandler { }

    // FIXME: This is probably not useful anymore.
    Connections {
        target: root.headerItem
        onFocusChanged: {
            if (!headerItem.focus) {
                currentItem.focus = true
            }
        }
    }

    MouseArea {
        anchors.fill: parent

        z: -1

        preventStealing: true

        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressed: {
            focus = true // Grab the focus from delegate
            root.forceActiveFocus(Qt.MouseFocusReason) // Re-focus the list

            if (!(mouse.modifiers & (Qt.ShiftModifier | Qt.ControlModifier))) {
                root.deselectAll()
            }

            mouse.accepted = true
        }

        onReleased: {
            if (mouse.button & Qt.RightButton) {
                root.showContextMenu(mapToGlobal(mouse.x, mouse.y))
            }
        }
    }
}
