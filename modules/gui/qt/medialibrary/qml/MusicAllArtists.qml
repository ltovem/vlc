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

import org.videolan.medialib 0.1
import org.videolan.vlc 0.1

import "qrc:///util/" as Util
import "qrc:///widgets/" as Widgets
import "qrc:///main/" as MainInterface
import "qrc:///util/Helpers.js" as Helpers
import "qrc:///style/"

MainInterface.MainViewLoader {
    id: root

    // Properties

    readonly property int currentIndex: Helpers.get(currentItem, "currentIndex", - 1)

    property Component header: null
    
    readonly property int contentLeftMargin: Helpers.get(currentItem, "contentLeftMargin", 0)
    readonly property int contentRightMargin: Helpers.get(currentItem, "contentRightMargin", 0)

    property alias parentId: artistModel.parentId
    property alias searchPattern: artistModel.searchPattern
    property alias sortOrder: artistModel.sortOrder
    property alias sortCriteria: artistModel.sortCriteria

    signal requestArtistAlbumView(int reason)

    isSearchable: true

    model: MLArtistModel {
        id: artistModel
        ml: MediaLib
    }

    sortModel: [
        { text: I18n.qtr("Alphabetic"), criteria: "name" },
        { text: I18n.qtr("Tracks Count"),   criteria: "nb_tracks" }
    ]

    grid: gridComponent
    list: tableComponent
    emptyLabel: emptyLabelComponent

    Util.MLContextMenu {
        id: contextMenu

        model: artistModel
    }

    Widgets.MLDragItem {
        id: artistsDragItem

        mlModel: artistModel
        indexes: indexesFlat ? selectionModel.selectedIndexesFlat
                             : selectionModel.selectedIndexes
        indexesFlat: !!selectionModel.selectedIndexesFlat
        defaultCover: VLCStyle.noArtArtistSmall
    }

    Component {
        id: gridComponent

        Widgets.ExpandGridItemView {
            id: artistGrid

            basePictureWidth: VLCStyle.gridCover_music_width
            basePictureHeight: VLCStyle.gridCover_music_height
            titleTopMargin: VLCStyle.gridItemTitle_topMargin + VLCStyle.margin_xxsmall
            
            selectionModel: root.selectionModel
            model: artistModel
            focus: true

            headerDelegate: root.header
            Navigation.parentItem: root

            onActionAtIndex: (index) => {
                if (selectionModel.selectedIndexes.length > 1) {
                    artistModel.addAndPlay( selectionModel.selectedIndexes )
                } else {
                    currentIndex = index
                    requestArtistAlbumView(Qt.TabFocusReason)
                }
            }

            delegate: AudioGridItem {
                id: gridItem

                width: artistGrid.cellWidth
                height: artistGrid.cellHeight

                pictureWidth: artistGrid.maxPictureWidth
                pictureHeight: artistGrid.maxPictureHeight
                pictureRadius: artistGrid.maxPictureWidth

                image: model.cover || ""
                fallbackImage: VLCStyle.noArtArtistSmall

                title: model.name || I18n.qtr("Unknown artist")
                subtitle: model.nb_tracks > 1 ? I18n.qtr("%1 songs").arg(model.nb_tracks) : I18n.qtr("%1 song").arg(model.nb_tracks)
                titleTopMargin: artistGrid.titleTopMargin
                playIconSize: VLCStyle.play_cover_small
                textAlignHCenter: true
                dragItem: artistsDragItem

                onItemClicked: (_,_, modifier) => { artistGrid.leftClickOnItem(modifier, index) }

                onItemDoubleClicked: (_,_, modifier) => { root.requestArtistAlbumView(Qt.MouseFocusReason) }

                onContextMenuButtonClicked: (_, globalMousePos) => {
                    artistGrid.rightClickOnItem(index)
                    contextMenu.popup(selectionModel.selectedIndexes, globalMousePos)
                }

                selectedShadow.anchors.margins: VLCStyle.dp(1) // outside border
                unselectedShadow.anchors.margins: VLCStyle.dp(1) // outside border
            }
        }
    }


    Component {
        id: tableComponent

        MainInterface.MainTableView {
            id: artistTable

            readonly property real _nbCols: VLCStyle.gridColumnsForWidth(artistTable.availableRowWidth)

            property var _modelSmall: [{
                size: Math.max(1.5, artistTable._nbCols),

                model: ({
                    criteria: "name",

                    subCriterias: [ "nb_tracks" ],

                    text: I18n.qtr("Name"),

                    headerDelegate: tableColumns.titleHeaderDelegate,
                    colDelegate: tableColumns.titleDelegate,

                    placeHolder: VLCStyle.noArtArtistSmall
                })
            }]

            property var _modelMedium: [{
                size: Math.max(1, artistTable._nbCols - 1),

                model: {
                    criteria: "name",

                    text: I18n.qtr("Name"),

                    headerDelegate: tableColumns.titleHeaderDelegate,
                    colDelegate: tableColumns.titleDelegate,

                    placeHolder: VLCStyle.noArtArtistSmall
                }
            }, {
                size: 1,

                model: {
                    criteria: "nb_tracks",

                    text: I18n.qtr("Tracks")
                }
            }]

            selectionModel: root.selectionModel
            model: artistModel
            focus: true
            dragItem: artistsDragItem
            rowHeight: VLCStyle.tableCoverRow_height

            header: root.header
            Navigation.parentItem: root

            onActionForSelection: (selection) => {
                artistModel.addAndPlay( selection )
                if ( selection.length === 1)
                    requestArtistAlbumView(Qt.TabFocusReason)
            }

            sortModel: (availableRowWidth < VLCStyle.colWidth(4)) ? _modelSmall
                                                                  : _modelMedium

            onItemDoubleClicked: root.requestArtistAlbumView(Qt.MouseFocusReason)

            onContextMenuButtonClicked: (_,_, globalMousePos) => {
                contextMenu.popup(selectionModel.selectedIndexes, globalMousePos)
            }
            onRightClick: (_,_,globalMousePos) => {
                contextMenu.popup(selectionModel.selectedIndexes, globalMousePos)
            }

            Widgets.TableColumns {
                id: tableColumns

                showCriterias: (artistTable.sortModel === artistTable._modelSmall)
            }
        }
    }

    Component {
        id: emptyLabelComponent

        Widgets.EmptyLabelButton {
            text: I18n.qtr("No artists found\nPlease try adding sources, by going to the Browse tab")
            Navigation.parentItem: root
            cover: VLCStyle.noArtArtistCover
        }
    }
}
