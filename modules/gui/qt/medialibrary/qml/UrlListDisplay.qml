
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
import QtQml.Models 2.12

import org.videolan.vlc 0.1
import org.videolan.medialib 0.1

import "qrc:///util" as Util
import "qrc:///widgets/" as Widgets
import "qrc:///style/"


Widgets.KeyNavigableTableView {
    id: listView_id

    readonly property bool isSearchable: false

    property string searchPattern
    property int sortOrder
    property string sortCriteria

    readonly property int _nbCols: VLCStyle.gridColumnsForWidth(
                                       listView_id.availableRowWidth)
    property Component urlHeaderDelegate: Widgets.IconLabel {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: VLCStyle.icon_tableHeader
        text: VLCIcons.history
        color: listView_id.colorContext.fg.secondary
    }

    visible: urlModel.count > 0

    model: MLUrlModel {
        ml: MediaLib

        searchPattern: listView_id.searchPattern
        sortOrder: listView_id.sortOrder
        sortCriteria: listView_id.sortCriteria
    }

    sortModel: [{
        size: Math.max(listView_id._nbCols - 1, 1),

        model: {
            criteria: "url",

            text: I18n.qtr("Url"),

            showSection: "url",

            headerDelegate: urlHeaderDelegate
        }
    }, {
        size: 1,

        model: {
            criteria: "last_played_date",

            text: I18n.qtr("Last played date"),

            showSection: "",
            showContextButton: true,

            headerDelegate: tableColumns.timeHeaderDelegate
        }
    }]

    rowHeight: VLCStyle.listAlbumCover_height + VLCStyle.margin_xxsmall * 2

    onActionForSelection: model.addAndPlay( selection )
    onItemDoubleClicked: MediaLib.addAndPlay(model.id)
    onContextMenuButtonClicked: contextMenu.popup(selectionModel.selectedIndexes, globalMousePos)
    onRightClick: contextMenu.popup(selectionModel.selectedIndexes, globalMousePos)

    Util.MLContextMenu {
        id: contextMenu

        model: listView_id.model
    }


    Widgets.TableColumns {
        id: tableColumns
    }
}
