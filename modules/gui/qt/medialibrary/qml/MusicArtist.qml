﻿/*****************************************************************************
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
import QtQuick.Controls 2.12
import QtQuick 2.12
import QtQml.Models 2.12
import QtQuick.Layouts 1.12

import org.videolan.medialib 0.1
import org.videolan.vlc 0.1

import "qrc:///util/" as Util
import "qrc:///util/Helpers.js" as Helpers
import "qrc:///widgets/" as Widgets
import "qrc:///main/" as MainInterface
import "qrc:///style/"

FocusScope {
    id: root

    property var artist: ({})

    //the index to "go to" when the view is loaded
    property int initialIndex: 0

    property Item headerItem: _currentView ? _currentView.headerItem : null

    property bool isSearchable: true

    property alias searchPattern: albumModel.searchPattern
    property alias sortOrder: albumModel.sortOrder
    property alias sortCriteria: albumModel.sortCriteria

    // current index of album model
    readonly property int currentIndex: {
        if (!_currentView)
           return -1
        else if (MainCtx.gridView)
           return _currentView.currentIndex
        else
           return headerItem.albumsListView.currentIndex
    }

    property alias rightPadding: view.rightPadding

    property alias _currentView: view.currentItem

    function navigationShowHeader(y, height) {
        const newContentY = Helpers.flickablePositionContaining(_currentView, y, height, 0, 0)

        if (newContentY !== _currentView.contentY)
            _currentView.contentY = newContentY
    }

    property Component header: FocusScope {
        id: headerFs

        property Item albumsListView: albumsLoader.status === Loader.Ready ? albumsLoader.item.albumsListView: null

        focus: true
        height: col.height
        width: root.width

        Navigation.parentItem: root
        Navigation.downAction: function() {
            const item = view.currentItem
            if (!item)
                return
            if (item.currentIndex === -1)
                item.currentIndex = 0
            console.assert(item.currentItem)
            item.currentItem.forceActiveFocus(Qt.TabFocusReason)
        }

        Column {
            id: col

            height: implicitHeight
            width: headerFs.width
            bottomPadding: VLCStyle.margin_normal

            ArtistTopBanner {
                id: artistBanner

                focus: true
                width: headerFs.width

                rightPadding: root.rightPadding

                artist: root.artist

                onActiveFocusChanged: {
                    // make sure content is visible with activeFocus
                    if (activeFocus)
                        root.navigationShowHeader(0, height)
                }

                Navigation.parentItem: headerFs
                Navigation.downItem: albumsListView
            }

            Loader {
                id: albumsLoader

                active: !MainCtx.gridView
                focus: true

                onActiveFocusChanged: {
                    // make sure content is visible with activeFocus
                    if (activeFocus)
                        root.navigationShowHeader(y, height)
                }

                sourceComponent: Column {
                    property alias albumsListView: albumsList

                    width: albumsList.width
                    height: implicitHeight

                    Widgets.SubtitleLabel {
                        id: albumsText

                        text: I18n.qtr("Albums")
                        color: theme.fg.primary
                        leftPadding: VLCStyle.margin_xlarge
                        topPadding: VLCStyle.margin_normal
                        bottomPadding: VLCStyle.margin_xsmall
                    }

                    Widgets.KeyNavigableListView {
                        id: albumsList

                        focus: true

                        height: VLCStyle.gridItem_music_height + topMargin + bottomMargin
                        width: root.width - root.rightPadding

                        leftMargin: VLCStyle.margin_xlarge
                        topMargin: VLCStyle.margin_xsmall
                        bottomMargin: VLCStyle.margin_xsmall
                        model: albumModel
                        selectionModel: albumSelectionModel
                        orientation: ListView.Horizontal
                        spacing: VLCStyle.column_spacing

                        Navigation.parentItem: headerFs

                        Navigation.upItem: artistBanner

                        delegate: Widgets.GridItem {
                            id: gridItem

                            image: model.cover || ""
                            fallbackImage: VLCStyle.noArtAlbumCover

                            title: model.title || I18n.qtr("Unknown title")
                            subtitle: model.release_year || ""
                            textAlignHCenter: true
                            x: selectedBorderWidth
                            y: selectedBorderWidth
                            pictureWidth: VLCStyle.gridCover_music_width
                            pictureHeight: VLCStyle.gridCover_music_height
                            playCoverBorderWidth: VLCStyle.gridCover_music_border
                            dragItem: albumDragItem

                            onPlayClicked: play()
                            onItemDoubleClicked: play()

                            onItemClicked: {
                                albumsList.selectionModel.updateSelection( modifier , albumsList.currentIndex, index )
                                albumsList.currentIndex = index
                                albumsList.forceActiveFocus()
                            }

                            Connections {
                                target: albumsList.selectionModel

                                onSelectionChanged: gridItem.selected = albumsList.selectionModel.isSelected(index)
                            }

                            function play() {
                                if ( model.id !== undefined ) {
                                    MediaLib.addAndPlay( model.id )
                                }
                            }
                        }

                        onActionAtIndex: albumModel.addAndPlay( new Array(index) )
                    }

                    Widgets.SubtitleLabel {
                        id: tracksText

                        text: I18n.qtr("Tracks")
                        color: theme.fg.primary
                        leftPadding: VLCStyle.margin_xlarge
                        topPadding: VLCStyle.margin_large
                    }
                }
            }
        }
    }

    focus: true

    onInitialIndexChanged: resetFocus()

    function resetFocus() {
        if (albumModel.count === 0) {
            return
        }
        let initialIndex = root.initialIndex
        if (initialIndex >= albumModel.count)
            initialIndex = 0
        albumSelectionModel.select(initialIndex, ItemSelectionModel.ClearAndSelect)
        const albumsListView = MainCtx.gridView ? _currentView : headerItem.albumsListView
        if (albumsListView) {
            albumsListView.currentIndex = initialIndex
            albumsListView.positionViewAtIndex(initialIndex, ItemView.Contain)
        }
    }

    function _actionAtIndex(index, model, selectionModel) {
        if (selectionModel.selectedIndexes.length > 1) {
            model.addAndPlay( selectionModel.selectedIndexes )
        } else {
            model.addAndPlay( new Array(index) )
        }
    }

    function _onNavigationCancel() {
        if (_currentView.currentIndex <= 0) {
            root.Navigation.defaultNavigationCancel()
        } else {
            _currentView.currentIndex = 0;
            _currentView.positionViewAtIndex(0, ItemView.Contain)
        }

        if (tableView_id.currentIndex <= 0)
            root.Navigation.defaultNavigationCancel()
        else
            tableView_id.currentIndex = 0;
    }

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.View
    }

    MLAlbumModel {
        id: albumModel

        ml: MediaLib
        parentId: artist.id

        onCountChanged: {
            if (albumModel.count > 0 && !albumSelectionModel.hasSelection) {
                root.resetFocus()
            }
        }
    }

    ListSelectionModel {
        id: albumSelectionModel
        model: albumModel
    }

    Widgets.MLDragItem {
        id: albumDragItem

        mlModel: albumModel
        indexes: indexesFlat ? albumSelectionModel.selectedIndexesFlat
                             : albumSelectionModel.selectedIndexes
        indexesFlat: !!albumSelectionModel.selectedIndexesFlat
        defaultCover: VLCStyle.noArtAlbumCover
    }

    MLAlbumTrackModel {
        id: trackModel

        ml: MediaLib
        parentId: albumModel.parentId
    }

    Util.MLContextMenu {
        id: contextMenu

        model: albumModel
    }

    Util.MLContextMenu {
        id: trackContextMenu

        model: trackModel
    }

    Component {
        id: gridComponent

        MainInterface.MainGridView {
            id: gridView_id

            focus: true
            activeFocusOnTab:true
            cellWidth: VLCStyle.gridItem_music_width
            cellHeight: VLCStyle.gridItem_music_height
            headerDelegate: root.header
            selectionModel: albumSelectionModel
            model: albumModel

            Connections {
                target: albumModel
                // selectionModel updates but doesn't trigger any signal, this forces selection update in view
                onParentIdChanged: currentIndex = -1
            }

            delegate: AudioGridItem {
                id: audioGridItem

                opacity: gridView_id.expandIndex !== -1 && gridView_id.expandIndex !== audioGridItem.index ? .7 : 1
                dragItem: albumDragItem

                onItemClicked : gridView_id.leftClickOnItem(modifier, index)

                onItemDoubleClicked: {
                    gridView_id.switchExpandItem(index)
                }

                onContextMenuButtonClicked: {
                    gridView_id.rightClickOnItem(index)
                    contextMenu.popup(albumSelectionModel.selectedIndexes, globalMousePos, { "information" : index})
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: VLCStyle.duration_short
                    }
                }
            }

            expandDelegate: MusicAlbumsGridExpandDelegate {
                id: expandDelegateId

                x: 0
                width: gridView_id.width
                onRetract: gridView_id.retract()
                Navigation.parentItem: root

                Navigation.cancelItem: gridView_id

                Navigation.upItem: gridView_id

                Navigation.downAction: function() {}
            }

            onActionAtIndex: {
                if (albumSelectionModel.selectedIndexes.length === 1) {
                    switchExpandItem(index);

                    expandItem.forceActiveFocus(Qt.TabFocusReason)
                } else {
                    _actionAtIndex(index, albumModel, albumSelectionModel);
                }
            }

            Navigation.parentItem: root

            Navigation.upItem: headerItem

            Navigation.cancelAction: root._onNavigationCancel

            Connections {
                target: contextMenu
                onShowMediaInformation: gridView_id.switchExpandItem( index )
            }
        }

    }

    Component {
        id: tableComponent

        MainInterface.MainTableView {
            id: tableView_id

            readonly property int _nbCols: VLCStyle.gridColumnsForWidth(tableView_id.availableRowWidth)

            clip: true // content may overflow if not enough space is provided
            model: trackModel

            onActionForSelection: {
                model.addAndPlay(selection)
            }

            header: root.header
            headerPositioning: ListView.InlineHeader
            rowHeight: VLCStyle.tableCoverRow_height

            property var _modelSmall: [{
                size: Math.max(2, tableView_id._nbCols),

                model: {
                    criteria: "title",

                    subCriterias: [ "duration", "album_title" ],

                    text: I18n.qtr("Title"),

                    headerDelegate: tableColumns.titleHeaderDelegate,
                    colDelegate: tableColumns.titleDelegate
                }
            }]

            property var _modelMedium: [{
                size: 2,

                model: {
                    criteria: "title",

                    text: I18n.qtr("Title"),

                    headerDelegate: tableColumns.titleHeaderDelegate,
                    colDelegate: tableColumns.titleDelegate
                }
            }, {
                size: Math.max(1, tableView_id._nbCols - 3),

                model: {
                    criteria: "album_title",

                    text: I18n.qtr("Album")
                }
            }, {
                size: 1,

                model: {
                    criteria: "duration",

                    text: I18n.qtr("Album"),

                    showSection: "",

                    headerDelegate: tableColumns.timeHeaderDelegate,
                    colDelegate: tableColumns.timeColDelegate
                }
            }]

            sortModel: (availableRowWidth < VLCStyle.colWidth(4)) ? _modelSmall
                                                                  : _modelMedium

            dragItem: tableDragItem

            Navigation.parentItem: root

            Navigation.upItem: headerItem

            Navigation.cancelAction: root._onNavigationCancel

            onItemDoubleClicked: MediaLib.addAndPlay(model.id)
            onContextMenuButtonClicked: trackContextMenu.popup(tableView_id.selectionModel.selectedIndexes, globalMousePos)
            onRightClick: trackContextMenu.popup(tableView_id.selectionModel.selectedIndexes, globalMousePos)

            onDragItemChanged: console.assert(tableView_id.dragItem === tableDragItem)

            Widgets.MLDragItem {
                id: tableDragItem

                mlModel: trackModel

                indexes: indexesFlat ? tableView_id.selectionModel.selectedIndexesFlat
                                     : tableView_id.selectionModel.selectedIndexes
                indexesFlat: !!tableView_id.selectionModel.selectedIndexesFlat

                defaultCover: VLCStyle.noArtArtistCover
            }

            Widgets.TableColumns {
                id: tableColumns

                showCriterias: (tableView_id.sortModel === tableView_id._modelSmall)
            }
        }
    }

    Widgets.StackViewExt {
        id: view

        anchors.fill: parent

        focus: albumModel.count !== 0
        initialItem: MainCtx.gridView ? gridComponent : tableComponent

        Connections {
            target: MainCtx
            onGridViewChanged: {
                if (MainCtx.gridView)
                    view.replace(gridComponent)
                else
                    view.replace(tableComponent)
            }
        }
    }
}
