/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQml.Models 2.2
import QtQml 2.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///util/" as Util
import "qrc:///style/"

Widgets.PageLoader {
    id: root

    property var sortModel
    property var contentModel
    property bool isViewMultiView: false

    pageModel: [{
            displayText: I18n.qtr("Services"),
            name: "services",
            url: "qrc:///network/ServicesHomeDisplay.qml"
        }, {
            displayText: I18n.qtr("URL"),
            name: "url",
            url: "qrc:///network/DiscoverUrlDisplay.qml"
        }
    ]

    loadDefaultView: function () {
        History.update(["mc", "discover", "services"])
        loadPage("services")
    }

    onCurrentItemChanged: {
        sortModel = currentItem.sortModel
        contentModel = currentItem.model
        localMenuDelegate = !!currentItem.localMenuDelegate ? currentItem.localMenuDelegate : menuDelegate
        isViewMultiView = currentItem.isViewMultiView === undefined || currentItem.isViewMultiView
    }


    function loadIndex(index) {
        History.push(["mc", "discover", root.pageModel[index].name])
    }


    property ListModel tabModel: ListModel {
        Component.onCompleted: {
            pageModel.forEach(function(e) {
                append({
                           displayText: e.displayText,
                           name: e.name,
                       })
            })
        }
    }

    property Component localMenuDelegate: menuDelegate

    Component {
        id: menuDelegate

        Widgets.LocalTabBar {
            currentView: root.view
            model: tabModel

            onClicked: root.loadIndex(index)
        }
    }
}
