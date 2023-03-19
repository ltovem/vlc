// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import org.videolan.vlc 0.1
import org.videolan.medialib 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"


Widgets.PageLoader {
    id: root

    property var sortModel
    property var contentModel
    property bool isViewMultiView: true

    pageModel: [{
            displayText: I18n.qtr("Artists"),
            name: "artists",
            url: "qrc:///medialibrary/MusicArtistsDisplay.qml"
        }, {
            displayText: I18n.qtr("Albums"),
            name: "albums",
            url: "qrc:///medialibrary/MusicAlbumsDisplay.qml"
        }, {
            displayText: I18n.qtr("Tracks"),
            name: "tracks" ,
            url: "qrc:///medialibrary/MusicTracksDisplay.qml"
        }, {
            displayText: I18n.qtr("Genres"),
            name: "genres" ,
            url: "qrc:///medialibrary/MusicGenresDisplay.qml"
        }, {
            displayText: I18n.qtr("Playlists"),
            name: "playlists" ,
            url: "qrc:///medialibrary/MusicPlaylistsDisplay.qml"
        }
    ]

    loadDefaultView: function () {
        History.update(["mc", "music", "artists"])
        loadPage("artists")
    }

    onCurrentItemChanged: {
        sortModel = currentItem.sortModel
        contentModel = currentItem.model
        isViewMultiView = currentItem.isViewMultiView === undefined || currentItem.isViewMultiView
    }

    function loadIndex(index) {
        History.push(["mc", "music", root.pageModel[index].name])
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

    property Component localMenuDelegate: Widgets.LocalTabBar {
        currentView: root.view
        model: tabModel

        onClicked: root.loadIndex(index)
    }
}
