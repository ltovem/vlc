/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick          2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts  1.11
import QtQml.Models     2.2

import org.videolan.vlc 0.1
import org.videolan.medialib 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Widgets.PageLoader {
    id: root

    //---------------------------------------------------------------------------------------------
    // Properties
    //---------------------------------------------------------------------------------------------

    property bool isViewMultiView: true

    property var contentModel

    property var sortMenu
    property var sortModel

    property ListModel tabModel: ListModel {
        Component.onCompleted: {
            pageModel.forEach(function(e) {
                append({
                    name       : e.name,
                    displayText: e.displayText
                })
            })
        }
    }

    property Component localMenuDelegate: Widgets.LocalTabBar {
        currentView: root.view

        model: tabModel

        onClicked: root.loadIndex(index)
    }

    //---------------------------------------------------------------------------------------------
    // Settings
    //---------------------------------------------------------------------------------------------

    pageModel: [{
            name: "all",
            displayText: I18n.qtr("All"),
            url: "qrc:///medialibrary/VideoAllDisplay.qml"
        },{
            name: "playlists",
            displayText: I18n.qtr("Playlists"),
            url: "qrc:///medialibrary/VideoPlaylistsDisplay.qml"
        }
    ]

    loadDefaultView: function () {
        History.update(["mc", "video", "all"])
        loadPage("all")
    }

    onCurrentItemChanged: {
        isViewMultiView = (currentItem.isViewMultiView === undefined
                           ||
                           currentItem.isViewMultiView);

        // NOTE: We need bindings because the VideoAll model can change over time.
        contentModel = Qt.binding(function () { return currentItem.model; })
        sortMenu     = Qt.binding(function () { return currentItem.sortMenu; })
        sortModel    = Qt.binding(function () { return currentItem.sortModel; })
    }

    //---------------------------------------------------------------------------------------------
    // Functions
    //---------------------------------------------------------------------------------------------

    function loadIndex(index) {
        History.push(["mc", "video", root.pageModel[index].name]);
    }
}
