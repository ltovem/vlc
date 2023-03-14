/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///util/" as Util
import "qrc:///main/" as MainInterface
import "qrc:///style/"

MainInterface.MainGridView {
    id: root

    readonly property bool isViewMultiView: false

    selectionDelegateModel: selectionModel
    model: sourcesFilterModel
    topMargin: VLCStyle.margin_large
    cellWidth: VLCStyle.gridItem_network_width
    cellHeight: VLCStyle.gridCover_network_height + VLCStyle.margin_xsmall + VLCStyle.fontHeight_normal

    delegate: Widgets.GridItem {

        property var model: ({})
        property int index: -1
        readonly property bool is_dummy: model.type === NetworkSourcesModel.TYPE_DUMMY

        title: is_dummy ? I18n.qtr("Add a service") : model.long_name
        subtitle: ""
        pictureWidth: VLCStyle.colWidth(1)
        pictureHeight: VLCStyle.gridCover_network_height
        height: VLCStyle.gridCover_network_height + VLCStyle.margin_xsmall + VLCStyle.fontHeight_normal
        playCoverBorderWidth: VLCStyle.gridCover_network_border
        playCoverShowPlay: false
        image: {
            if (is_dummy) {
                return SVGColorImage.colorize("qrc:///placeholder/add_service.svg")
                    .color1(this.colorContext.fg.secondary)
                    .accent(this.colorContext.accent)
                    .uri()
            } else if (model.artwork && model.artwork.toString() !== "") {
                //if the source is a qrc artwork, we should colorize it
                if (model.artwork.toString().match(/qrc:\/\/.*svg/))
                {
                    return SVGColorImage.colorize(model.artwork)
                        .color1(this.colorContext.fg.secondary)
                        .accent(this.colorContext.accent)
                        .uri()
                }
                return model.artwork
            } else {
                return SVGColorImage.colorize("qrc:///sd/directory.svg")
                           .color1(this.colorContext.fg.secondary)
                           .uri()
            }
        }

        onItemDoubleClicked: {
            if (is_dummy)
                History.push(["mc", "discover", "services", "services_manage"]);
            else
                History.push(["mc", "discover", "services", "source_root",
                              { source_name: model.name }]);

            root.setCurrentItemFocus(Qt.MouseFocusReason);
        }

        onItemClicked : {
            selectionModel.updateSelection(modifier , root.currentIndex, index)
            root.currentIndex = index
            root.forceActiveFocus()
        }
    }

    onActionAtIndex: {
        var itemData = sourcesFilterModel.getDataAt(index);

        if (itemData.type === NetworkSourcesModel.TYPE_DUMMY)
            History.push(["mc", "discover", "services", "services_manage"]);
        else
            History.push(["mc", "discover", "services", "source_root",
                          { source_name: itemData.name }]);

        root.setCurrentItemFocus(Qt.TabFocusReason);
    }

    Navigation.parentItem: root

    Navigation.cancelAction: function() {
        History.previous();

        root.setCurrentItemFocus(Qt.TabFocusReason);
    }

    NetworkSourcesModel {
        id: sourcesModel

        ctx: MainCtx
    }

    Util.SelectableDelegateModel {
        id: selectionModel

        model: sourcesFilterModel
    }

    SortFilterProxyModel {
        id: sourcesFilterModel

        sourceModel: sourcesModel
        searchRole: "name"
    }
}
