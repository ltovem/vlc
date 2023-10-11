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

import QtQml.Models 2.12

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///util/Helpers.js" as Helpers
import "qrc:///util/" as Util

FocusScope {
    id: root

    // Properties

    /// cell Width
    property int cellWidth: 100
    // cell Height
    property int cellHeight: 100

    //margin to apply
    property int bottomMargin: 0
    property int topMargin: 0
    property int leftMargin: VLCStyle.column_margin + leftPadding
    property int rightMargin: VLCStyle.column_margin + rightPadding

    property int leftPadding: 0
    property int rightPadding: 0

    readonly property int extraMargin: (_contentWidth - nbItemPerRow * _effectiveCellWidth
                                        +
                                        horizontalSpacing) / 2

    // NOTE: The grid margins for the item(s) horizontal positioning.
    readonly property int contentLeftMargin: extraMargin + leftMargin
    readonly property int contentRightMargin: extraMargin + rightMargin

    readonly property int rowHeight: cellHeight + verticalSpacing

    property int rowX: 0
    property int horizontalSpacing: VLCStyle.column_spacing
    property int verticalSpacing: VLCStyle.column_spacing

    property int displayMarginEnd: 0

    readonly property int nbItemPerRow: Math.max(Math.floor((_contentWidth + horizontalSpacing)
                                                            /
                                                            _effectiveCellWidth), 1)

    readonly property int _effectiveCellWidth: cellWidth + horizontalSpacing

    readonly property int _contentWidth: width - rightMargin - leftMargin

    property Util.SelectableDelegateModel selectionDelegateModel
    property QtAbstractItemModel model

    property int currentIndex: 0

    property bool isAnimating: animateRetractItem.running || animateExpandItem.running

    property int _count: 0

    property bool _isInitialised: false

    property bool _releaseActionButtonPressed: false

    /// the id of the item to be expanded
    property int expandIndex: -1
    property int _newExpandIndex: -1
    property int _expandItemVerticalSpace: 0

    property int _currentFocusReason: Qt.OtherFocusReason

    //delegate to display the extended item
    property Component delegate: Item{}

    property var _idChildrenList: []
    property var _unusedItemList: []
    property var _currentRange: [0,0]

    // Aliases

    property alias contentHeight: flickable.contentHeight
    property alias contentWidth: flickable.contentWidth
    property alias contentX: flickable.contentX
    property alias gridScrollBar: flickableScrollBar

    property alias expandDelegate: expandItemLoader.sourceComponent
    property alias expandItem: expandItemLoader.item

    property alias headerDelegate: headerItemLoader.sourceComponent
    property alias headerHeight: headerItemLoader.implicitHeight
    property alias headerItem: headerItemLoader.item

    property alias footerDelegate: footerItemLoader.sourceComponent
    property alias footerItem: footerItemLoader.item

    // Signals

    //signals emitted when selected items is updated from keyboard
    signal selectAll()
    signal actionAtIndex(int index)

    signal showContextMenu(point globalPos)

    // Settings

    contentWidth: {
        const size = _effectiveCellWidth * nbItemPerRow - horizontalSpacing

        return leftMargin + size + rightMargin
    }

    contentHeight: {
        const size = getItemPos(_count - 1)[1] + rowHeight + _expandItemVerticalSpace

        // NOTE: topMargin and headerHeight are included in root.getItemPos.
        if (footerItem)
            return size + footerItem.height + bottomMargin
        else
            return size + bottomMargin
    }

    Accessible.role: Accessible.Table

    activeFocusOnTab: true

    // Events

    Component.onCompleted: flickable.layout(true)

    onHeightChanged: flickable.layout(false)

    // NOTE: Update on contentLeftMargin since we depend on this for item placements.
    onContentLeftMarginChanged: flickable.layout(true)

    onDisplayMarginEndChanged: flickable.layout(false)

    onModelChanged: _onModelCountChanged()

    onCurrentIndexChanged: {
        if (expandIndex !== -1)
            retract()
        positionViewAtIndex(currentIndex, ItemView.Contain)
    }

    on_ExpandItemVerticalSpaceChanged: {
        if (expandItem) {
            expandItem.visible = _expandItemVerticalSpace - verticalSpacing > 0
            expandItem.height = Math.max(_expandItemVerticalSpace - verticalSpacing, 0)
        }
        flickable.layout(true)
    }

    // Keys

    Keys.onPressed: (event) => {
        let newIndex = -1
        if (KeyHelper.matchRight(event)) {
            if ((currentIndex + 1) % nbItemPerRow !== 0) {//are we not at the end of line
                newIndex = Math.min(_count - 1, currentIndex + 1)
            }
        } else if (KeyHelper.matchLeft(event)) {
            if (currentIndex % nbItemPerRow !== 0) {//are we not at the beginning of line
                newIndex = Math.max(0, currentIndex - 1)
            }
        } else if (KeyHelper.matchDown(event)) {
            const lastIndex = _count - 1
            // we are not on the last line
            if (Math.floor(currentIndex / nbItemPerRow)
                !==
                Math.floor(lastIndex / nbItemPerRow)) {
                newIndex = Math.min(lastIndex, currentIndex + nbItemPerRow)
            }
        } else if (KeyHelper.matchPageDown(event)) {
            newIndex = Math.min(_count - 1, currentIndex + nbItemPerRow * 5)
        } else if (KeyHelper.matchUp(event)) {
            if (Math.floor(currentIndex / nbItemPerRow) !== 0) { //we are not on the first line
                newIndex = Math.max(0, currentIndex - nbItemPerRow)
            }
        } else if (KeyHelper.matchPageUp(event)) {
            newIndex = Math.max(0, currentIndex - nbItemPerRow * 5)
        } else if (KeyHelper.matchOk(event) || event.matches(StandardKey.SelectAll) ) {
            //these events are matched on release
            event.accepted = true
        }

        if (event.matches(StandardKey.SelectAll) || KeyHelper.matchOk(event)) {
            _releaseActionButtonPressed = true
        } else {
            _releaseActionButtonPressed = false
        }

        if (newIndex !== -1 && newIndex !== currentIndex) {
            event.accepted = true;

            const oldIndex = currentIndex;
            currentIndex = newIndex;
            selectionDelegateModel.updateSelection(event.modifiers, oldIndex, newIndex)

            // NOTE: We make sure we have the proper visual focus on components.
            if (oldIndex < currentIndex)
                setCurrentItemFocus(Qt.TabFocusReason);
            else
                setCurrentItemFocus(Qt.BacktabFocusReason);
        }

        if (!event.accepted) {
            Navigation.defaultKeyAction(event)
        }
    }

    Keys.onReleased: (event) => {
        if (!_releaseActionButtonPressed)
            return

        if (event.matches(StandardKey.SelectAll)) {
            event.accepted = true
            selectionDelegateModel.select(model.index(0, 0), ItemSelectionModel.Select | ItemSelectionModel.Columns)
        } else if ( KeyHelper.matchOk(event) ) {
            event.accepted = true
            actionAtIndex(currentIndex)
        }
        _releaseActionButtonPressed = false
    }

    // Connections

    Connections {
        target: root.model
        onDataChanged: {
            const iMin = topLeft.row
            const iMax = bottomRight.row + 1 // [] => [)
            const f_l = _currentRange
            if (iMin < f_l[1] && f_l[0] < iMax) {
                root._refreshData(iMin, iMax)
            }
        }
        onRowsInserted: root._onModelCountChanged()
        onRowsRemoved: root._onModelCountChanged()
        onModelReset: root._onModelCountChanged()

        // NOTE: This is useful for SortFilterProxyModel(s).
        onLayoutChanged: root._onModelCountChanged()
    }

    Connections {
        target: root.selectionDelegateModel

        onSelectionChanged: {
            for (let i = 0; i < selected.length; ++i) {
                root._updateSelectedRange(selected[i].topLeft, selected[i].bottomRight, true)
            }

            for (let i = 0; i < deselected.length; ++i) {
                root._updateSelectedRange(deselected[i].topLeft, deselected[i].bottomRight, false)
            }
        }
    }

    Connections {
        target: MainCtx
        onIntfScaleFactorChanged: flickable.layout(true)
    }

    // Animations

    PropertyAnimation {
        id: animateContentY
        target: flickable
        properties: "contentY"
    }

    // Functions

    // NOTE: This function is useful to set the currentItem without losing the visual focus.
    function setCurrentItem(index) {
        if (currentIndex === index)
            return

        let reason

        let item = _getItem(index)

        if (item)
            reason = item.focusReason
        else
            reason = _currentFocusReason

        currentIndex = index

        item = _getItem(index)

        if (reason !== Qt.OtherFocusReason) {
            if (item)
                Helpers.enforceFocus(item, reason)
            else
                setCurrentItemFocus(reason)
        }
    }

    function setCurrentItemFocus(reason) {

        // NOTE: Saving the focus reason for later.
        _currentFocusReason = reason;

        if (!model || model.count === 0 || currentIndex === -1) {
            // NOTE: By default we want the focus on the flickable.
            flickable.forceActiveFocus(reason);

            return;
        }

        if (_containsItem(currentIndex))
            Helpers.enforceFocus(_getItem(currentIndex), reason);
        else
            flickable.forceActiveFocus(reason);

        // NOTE: We make sure the current item is fully visible.
        positionViewAtIndex(currentIndex, ItemView.Contain);

        if (expandIndex !== -1) {
            // We clear expandIndex so we can apply the proper focus in _setupChild.
            retract();
        }
    }

    function switchExpandItem(index) {
        if (_count === 0)
            return

        if (index === expandIndex)
            _newExpandIndex = -1
        else
            _newExpandIndex = index

        if (expandIndex !== -1)
            flickable.retract()
        else
            flickable.expand()
    }

    function retract() {
        _newExpandIndex = -1
        flickable.retract()
    }

    function getItemY(index) {
        return Math.floor(index / nbItemPerRow) * rowHeight + headerHeight + topMargin
    }

    function getItemRowCol(id) {
        const rowId = Math.floor(id / nbItemPerRow)
        const colId = id % nbItemPerRow
        return [colId, rowId]
    }

    function getItemPos(id) {
        const rowCol = getItemRowCol(id);

        const x = rowCol[0] * _effectiveCellWidth + contentLeftMargin;

        const y = rowCol[1] * rowHeight + headerHeight + topMargin;

        // NOTE: Position needs to be integer based if we want to avoid visual artifacts like
        //       wrong alignments or blurry texture rendering.
        return [Math.round(x), Math.round(y)];
    }

    //use the same signature as Gridview.positionViewAtIndex(index, PositionMode mode)
    //mode is ignored at the moment
    function positionViewAtIndex(index, mode) {
        if (flickable.width === 0 || flickable.height === 0
            ||
            index < 0 || index >= _count)
            return

        const newContentY = Helpers.flickablePositionContaining(flickable,
                                                                getItemPos(index)[1]
                                                                , rowHeight
                                                                , topMargin, bottomMargin)

        if (newContentY !== flickable.contentY)
            animateFlickableContentY(newContentY)
    }

    function leftClickOnItem(modifier, index) {
        selectionDelegateModel.updateSelection(modifier, currentIndex, index)
        if (selectionDelegateModel.isSelected(model.index(index, 0)))
            currentIndex = index
        else if (currentIndex === index) {
            if (_containsItem(currentIndex))
                _getItem(currentIndex).focus = false
            currentIndex = -1
        }

        // NOTE: We make sure to clear the keyboard focus.
        flickable.forceActiveFocus();
    }

    function rightClickOnItem(index) {
        if (!selectionDelegateModel.isSelected(model.index(index, 0))) {
            leftClickOnItem(Qt.NoModifier, index)
        }
    }

    function animateFlickableContentY( newContentY ) {
        animateContentY.stop()
        animateContentY.duration = VLCStyle.duration_long
        animateContentY.to = newContentY
        animateContentY.start()
    }

    // Private

    function _initialize() {
        if (_isInitialised)
            return;

        if (flickable.width === 0 || flickable.height === 0)
            return;
        if (currentIndex !== 0)
            positionViewAtIndex(currentIndex, ItemView.Contain)
        _isInitialised = true;
    }

    function _updateSelectedRange(topLeft, bottomRight, select) {
        let iMin = topLeft.row
        let iMax = bottomRight.row + 1 // [] => [)
        if (iMin < root._currentRange[1] && root._currentRange[0] < iMax) {
            iMin = Math.max(iMin, root._currentRange[0])
            iMax = Math.min(iMax, root._currentRange[1])
            for (let j = iMin; j < iMax; j++) {
                const item = root._getItem(j)
                console.assert(item)
                item.selected = select
            }
        }
    }

    function _calculateCurrentRange() {
        const myContentY = flickable.contentY
        let contentYWithoutExpand = myContentY
        let heightWithoutExpand = flickable.height + displayMarginEnd

        if (expandIndex !== -1) {
            const expandItemY = getItemPos(flickable.getExpandItemGridId())[1]

            if (myContentY >= expandItemY && myContentY < expandItemY + _expandItemVerticalSpace)
                contentYWithoutExpand = expandItemY
            if (myContentY >= expandItemY + _expandItemVerticalSpace)
                contentYWithoutExpand = myContentY - _expandItemVerticalSpace

            const expandYStart = Math.max(myContentY, expandItemY)
            const expandYEnd = Math.min(myContentY + height, expandItemY + _expandItemVerticalSpace)
            const expandDisplayedHeight = Math.max(expandYEnd - expandYStart, 0)
            heightWithoutExpand -= expandDisplayedHeight
        }

        const onlyGridContentY = contentYWithoutExpand - headerHeight - topMargin
        const firstRowId = Math.floor(onlyGridContentY / rowHeight)
        const firstId = Math.max(firstRowId * nbItemPerRow, 0)

        const lastRowId = Math.ceil((onlyGridContentY + heightWithoutExpand) / rowHeight)
        const lastId = Math.min(lastRowId * nbItemPerRow, _count)

        return [firstId, lastId]
    }

    function _getItem(id) {
        const i = id - _currentRange[0]
        return _idChildrenList[i]
    }

    function _setItem(id, item) {
        const i = id - _currentRange[0]
        _idChildrenList[i] = item
    }

    function _containsItem(id) {
        const i = id - _currentRange[0]
        const childrenList = _idChildrenList
        return i >= 0 && i < childrenList.length && typeof childrenList[i] !== "undefined"
    }

    function _indexToZ(id) {
        const rowCol = getItemRowCol(id)
        return rowCol[0] % 2 + 2 * (rowCol[1] % 2)
    }

    function _repositionItem(id, x, y) {
        const item = _getItem(id)
        console.assert(item !== undefined, "wrong child: " + id)

        //theses properties are always defined in Item
        item.x = x
        item.y = y
        item.z = _indexToZ(id)
        item.selected = selectionDelegateModel.isSelected(model.index(id, 0))

        return item
    }

    function _recycleItem(id, x, y) {
        const item = _unusedItemList.pop()
        console.assert(item !== undefined, "incorrect _recycleItem call, id" + id + " ununsedItemList size" + _unusedItemList.length)

        item.index = id
        item.model = model.getDataAt(id)
        item.selected = selectionDelegateModel.isSelected(model.index(id, 0))
        item.x = x
        item.y = y
        item.z = _indexToZ(id)
        item.visible = true

        _setItem(id, item)

        return item
    }

    function _createItem(id, x, y) {
        const item = delegate.createObject( flickable.contentItem, {
                        selected: selectionDelegateModel.isSelected(model.index(id, 0)),
                        index: id,
                        model: model.getDataAt(id),
                        x: x,
                        y: y,
                        z: _indexToZ(id),
                        visible: true
                    })

        console.assert(item !== undefined, "unable to instantiate " + id)
        _setItem(id, item)

        return item
    }

    function _setupChild(id, ydelta) {
        const pos = getItemPos(id)

        let item;

        if (_containsItem(id))
            item = _repositionItem(id, pos[0], pos[1] + ydelta)
        else if (_unusedItemList.length > 0)
            item = _recycleItem(id, pos[0], pos[1] + ydelta)
        else
            item = _createItem(id, pos[0], pos[1] + ydelta)

        // NOTE: This makes sure we have the proper focus reason on the GridItem.
        if (activeFocus && currentIndex === item.index && expandIndex === -1)
            item.forceActiveFocus(_currentFocusReason)
        else
            item.focus = false
    }

    function _refreshData( iMin, iMax ) {
        const f_l = _currentRange
        if (!iMin || iMin < f_l[0])
            iMin = f_l[0]
        if (!iMax || iMax > f_l[1])
            iMax= f_l[1]

        for (let id  = iMin; id < iMax; id++) {
            const item = _getItem(id)
            item.model = model.getDataAt(id)
        }

        if (expandIndex >= iMin && expandIndex < iMax) {
            expandItem.model = model.getDataAt(expandIndex)
        }
    }

    function _onModelCountChanged() {
        _count = model ? model.rowCount() : 0
        if (!_isInitialised)
            return

        // Hide the expandItem with no animation
        expandIndex = -1
        _expandItemVerticalSpace = 0

        // Regenerate the gridview layout
        flickable.layout(true)
        _refreshData()
    }

    // Children

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.View
    }


    Flickable {
        id: flickable

        anchors.fill: parent

        flickableDirection: Flickable.VerticalFlick

        ScrollBar.vertical: ScrollBar {
            id: flickableScrollBar
        }

        onContentYChanged: { Qt.callLater(flickable.layout, false) }


        MouseArea {
            anchors.fill: parent
            z: -1

            preventStealing: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onPressed: (mouse) => {
                Helpers.enforceFocus(flickable, Qt.MouseFocusReason)

                if (!(mouse.modifiers & (Qt.ShiftModifier | Qt.ControlModifier))) {
                    if (selectionDelegateModel)
                        selectionDelegateModel.clear()
                }
            }

            onReleased: (mouse) => {
                if (mouse.button & Qt.RightButton) {
                    root.showContextMenu(mapToGlobal(mouse.x, mouse.y))
                }
            }
        }

        Util.FlickableScrollHandler { }

        Loader {
            id: headerItemLoader

            x: 0
            y: root.topMargin

            //load the header early (when the first row is visible)
            visible: flickable.contentY < (root.headerHeight + root.rowHeight + root.topMargin)

            focus: (status === Loader.Ready) ? item.focus : false
        }

        Loader {
            id: footerItemLoader

            focus: (status === Loader.Ready) ? item.focus : false

            y: root.topMargin + root.headerHeight + (root.rowHeight * (Math.ceil(root.model.count / root.nbItemPerRow))) +
               root._expandItemVerticalSpace
        }

        Connections {
            target: root.headerItem

            onHeightChanged: {
                flickable.layout(true)
            }

            onActiveFocusChanged: {
                // when header loads because of setting headerItem.focus == true, it will suddenly attain the active focus
                // but then a queued flickable.layout() may take away it's focus and assign it to current item,
                // using Qt.callLater we save unnecessary scrolling
                Qt.callLater(flickable._scrollToHeaderOnFocus)
            }
        }

        Connections {
            target: root.footerItem
            onHeightChanged: {
                if (flickable.contentY + flickable.height > footerItemLoader.y + footerItemLoader.height)
                    flickable.contentY = footerItemLoader.y + footerItemLoader.height - flickable.height
                flickable.layout(false)
            }
        }

        Connections {
            target: root.expandItem
            onImplicitHeightChanged: {
                /* This is the only event we have after the expandItem height content was resized.
                   We can trigger here the expand animation with the right final height. */
                if (root.expandIndex !== -1)
                    flickable.expandAnimation()
            }
        }

        Loader {
            id: expandItemLoader

            active: root.expandIndex !== -1
            focus: active

            onLoaded: {
                item.height = 0

                // only make loader visible after setting initial y pos from layout() as to not get flickering
                expandItemLoader.visible = false
            }
        }

        function _scrollToHeaderOnFocus() {
            if (!root.headerItem.activeFocus)
                return;

            // when we gain the focus ensure the widget is fully visible
            animateFlickableContentY(0)
        }

        function getExpandItemGridId() {
            if (root.expandIndex !== -1) {
                const rowCol = root.getItemRowCol(root.expandIndex)
                const rowId = rowCol[1] + 1
                return rowId * root.nbItemPerRow
            } else {
                return root._count
            }
        }

        function _setupIndexes(force, range, yDelta) {
            for (let i = range[0]; i < range[1]; i++) {
                if (!force && root._containsItem(i))
                    continue
                _setupChild(i, yDelta)
            }
        }

        function _overlappedInterval(i1, i2) {
            if (i1[0] > i2[0]) return _overlappedInterval(i2, i1)
            if (i1[1] > i2[0]) return [i2[0], Math.min(i1[1], i2[1])]
            return [0, 0]
        }

        function _updateChildrenMap(first, last) {
            if (first >= last) {
                root._idChildrenList.forEach(function(item) { item.visible = false; })
                root._unusedItemList = root._idChildrenList
                root._idChildrenList = []
                root._currentRange = [0, 0]
                return
            }

            const overlapped = _overlappedInterval([first, last], root._currentRange)

            const newList = new Array(last - first)

            for (let i = overlapped[0]; i < overlapped[1]; ++i) {
                newList[i - first] = root._getItem(i)
                root._setItem(i, undefined)
            }

            for (let i = root._currentRange[0]; i < root._currentRange[1]; ++i) {
                const item = root._getItem(i)
                if (typeof item !== "undefined") {
                    item.visible = false
                    root._unusedItemList.push(item)
                    //  root._setItem(i, undefined) // not needed the list will be reset following this loop
                }
            }

            root._idChildrenList = newList
            root._currentRange = [first, last]
        }

        function layout(forceRelayout) {
            if (flickable.width === 0 || flickable.height === 0)
                return
            else if (!root._isInitialised)
                root._initialize()

            root.rowX = root.getItemPos(0)[0]

            const expandItemGridId = getExpandItemGridId()

            const f_l = root._calculateCurrentRange()
            const nbItems = f_l[1] - f_l[0]
            const firstId = f_l[0]
            const lastId = f_l[1]

            const topGridEndId = Math.max(Math.min(expandItemGridId, lastId), firstId)

            if (!forceRelayout && root._currentRange[0] === firstId && root._currentRange[1] === lastId)
                return;

            _updateChildrenMap(firstId, lastId)

            // Place the delegates before the expandItem
            _setupIndexes(forceRelayout, [firstId, topGridEndId], 0)

            if (root.expandIndex !== -1) {
                const expandItemPos = root.getItemPos(expandItemGridId)
                expandItem.y = expandItemPos[1]
                if (!expandItemLoader.visible)
                    expandItemLoader.visible = true
            }

            // Place the delegates after the expandItem
            _setupIndexes(forceRelayout, [topGridEndId, lastId], root._expandItemVerticalSpace)
        }

        function expand() {
            root.expandIndex = root._newExpandIndex
            if (root.expandIndex === -1)
                return
            root.expandItem.model = root.model.getDataAt(root.expandIndex)
            /* We must also start the expand animation here since the expandItem implicitHeight is not
               changed if it had the same height at previous opening. */
            expandAnimation()
        }

        function expandAnimation() {
            if (root.expandIndex === -1)
                return

            const expandItemHeight = root.expandItem.implicitHeight + root.verticalSpacing

            // Expand animation

            expandItem.focus = true
            // The animation may have already been triggered, we must stop it.
            animateExpandItem.stop()
            animateExpandItem.from = root._expandItemVerticalSpace
            animateExpandItem.to = expandItemHeight
            animateExpandItem.start()

            // Sliding animation
            const currentItemYPos = root.getItemPos(root.expandIndex)[1]
                                    + root.rowHeight / 2
            animateFlickableContentY(currentItemYPos)
        }

        function retract() {
            animateRetractItem.start()
        }

        NumberAnimation {
            id: animateRetractItem;
            target: root;
            properties: "_expandItemVerticalSpace"
            easing.type: Easing.OutQuad
            duration: VLCStyle.duration_long
            to: 0
            onStopped: {
                root.expandIndex = -1
                if (root._newExpandIndex !== -1)
                    flickable.expand()
            }
        }

        NumberAnimation {
            id: animateExpandItem;
            target: root;
            properties: "_expandItemVerticalSpace"
            easing.type: Easing.InQuad
            duration: VLCStyle.duration_long
            from: 0
        }
    }
}
