/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQml.Models 2.2
import QtQml 2.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Widgets.PageLoader {
    id: root

    // Properties

    property var sortModel
    property var contentModel
    property bool isViewMultiView: true

    property Component localMenuDelegate

    // Settings

    pageModel: [{
        name: "home",
        url: "qrc:///network/BrowseHomeDisplay.qml"
    }, {
        name: "folders",
        component: browseFolders,
    }, {
        name: "device",
        component: browseDevice,
    }, {
        name: "browse",
        component: browseComponent,
        guard: function (prop) { return !!prop.tree }
    }]

    loadDefaultView: function() {
        History.update(["mc", "network", "home"])
        loadPage("home")
    }

    // Events
    onCurrentItemChanged: {
        sortModel = currentItem.sortModel;
        contentModel = currentItem.model;

        isViewMultiView = (currentItem.isViewMultiView === undefined
                           ||
                           currentItem.isViewMultiView);

        if (view.name === "home")
            localMenuDelegate = null
        else
            localMenuDelegate = componentBar
    }

    // Connections
    Connections {
        target: (stackView.currentItem instanceof BrowseHomeDisplay) ? stackView.currentItem
                                                                     : null

        onSeeAll: {
            if (sd_source === -1)
                History.push(["mc", "network", "folders", { title: title }])
            else
                History.push(["mc", "network", "device", { title: title, sd_source: sd_source }])

            stackView.currentItem.setCurrentItemFocus(reason)
        }
    }

    Connections {
        target: stackView.currentItem

        onBrowse: {
            History.push(["mc", "network", "browse", { tree: tree }])

            stackView.currentItem.setCurrentItemFocus(reason)
        }
    }

    // Children

    Component {
        id: browseFolders

        BrowseDeviceView {
            property var sortModel: [
                { text: I18n.qtr("Alphabetic"), criteria: "name" },
                { text: I18n.qtr("Url"),        criteria: "mrl"  }
            ]

            displayMarginEnd: g_mainDisplay.displayMargin

            model: modelFilter

            sourceModel: StandardPathModel {}
        }
    }

    Component {
        id: browseDevice

        BrowseDeviceView {
            id: viewDevice

            property var sd_source

            property var sortModel: [
                { text: I18n.qtr("Alphabetic"), criteria: "name" },
                { text: I18n.qtr("Url"),        criteria: "mrl"  }
            ]

            displayMarginEnd: g_mainDisplay.displayMargin

            model: modelFilter

            sourceModel: NetworkDeviceModel {
                ctx: MainCtx

                sd_source: viewDevice.sd_source
                source_name: "*"
            }
        }
    }

    Component {
        id: browseComponent

        BrowseTreeDisplay {
            providerModel: NetworkMediaModel {
                ctx: MainCtx
            }

            contextMenu: NetworkMediaContextMenu {
                model: providerModel
            }

            Navigation.cancelAction: function() {
                History.previous()

                stackView.currentItem.setCurrentItemFocus(Qt.BacktabFocusReason)
            }
        }
    }

    Component {
        id: componentBar

        NetworkAddressbar {
            path: view.name === "browse" ? root.stackView.currentItem.providerModel.path : []

            onHomeButtonClicked: {
                History.push(["mc", "network", "home"])

                stackView.currentItem.setCurrentItemFocus(reason)
            }

            onBrowse: {
                History.push(["mc", "network", "browse", { "tree": tree }])

                stackView.currentItem.setCurrentItemFocus(reason)
            }
        }
    }
}
