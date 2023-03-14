
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQml.Models 2.2

import org.videolan.vlc 0.1
import org.videolan.medialib 0.1

import "qrc:///util" as Util
import "qrc:///widgets/" as Widgets
import "qrc:///style/"


Widgets.KeyNavigableTableView {
    id: listView_id

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
    model: urlModel
    selectionDelegateModel: selectionModel

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

            showSection: "",
            showContextButton: true,

            headerDelegate: tableColumns.timeHeaderDelegate
        }
    }]

    rowHeight: VLCStyle.listAlbumCover_height + VLCStyle.margin_xxsmall * 2

    onActionForSelection: MediaLib.addAndPlay(model.getIdsForIndexes(
                                                  selection))
    onItemDoubleClicked: MediaLib.addAndPlay(model.id)
    onContextMenuButtonClicked: contextMenu.popup(selectionModel.selectedIndexes, globalMousePos)
    onRightClick: contextMenu.popup(selectionModel.selectedIndexes, globalMousePos)

    MLUrlModel {
        id: urlModel

        ml: MediaLib
    }

    Util.SelectableDelegateModel {
        id: selectionModel
        model: urlModel
    }

    Util.MLContextMenu {
        id: contextMenu

        model: urlModel
    }


    Widgets.TableColumns {
        id: tableColumns
    }
}
