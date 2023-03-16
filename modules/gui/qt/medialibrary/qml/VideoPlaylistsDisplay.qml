// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
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
    // Aliases
    //---------------------------------------------------------------------------------------------

    property bool isViewMultiView: true

    property var model
    property var sortModel

    //---------------------------------------------------------------------------------------------
    // Settings
    //---------------------------------------------------------------------------------------------

    pageModel: [{
        name: "all",
        component: componentAll
    }, {
        name: "list",
        component: componentList
    }]

    loadDefaultView: function () {
        History.update(["mc", "video", "playlists", "all"])
        loadPage("all")
    }

    //---------------------------------------------------------------------------------------------
    // Events
    //---------------------------------------------------------------------------------------------

    onCurrentItemChanged: {
        model     = currentItem.model;
        sortModel = currentItem.sortModel;

        isViewMultiView = (currentItem.isViewMultiView === undefined
                           ||
                           currentItem.isViewMultiView);
    }

    //---------------------------------------------------------------------------------------------
    // Functions
    //---------------------------------------------------------------------------------------------
    // Private

    function _updateHistoryList(index) {
        History.update(["mc", "video", "playlists", "all", { "initialIndex": index }]);
    }

    function _updateHistoryPlaylist(playlist) {
        History.update(["mc", "video", "playlists", "list", {
                            "currentIndex": playlist.currentIndex,
                            "parentId"   : playlist.parentId,
                            "name" : playlist.name
                        }]);
    }

    //---------------------------------------------------------------------------------------------
    // Childs
    //---------------------------------------------------------------------------------------------

    Component {
        id: componentAll

        PlaylistMediaList {

            isMusic: false

            onCurrentIndexChanged: _updateHistoryList(currentIndex)

            onShowList: {
                History.push(["mc", "video", "playlists", "list",
                             { parentId: model.id, name: model.name }]);

                stackView.currentItem.setCurrentItemFocus(reason);
            }
        }
    }

    Component {
        id: componentList

        PlaylistMediaDisplay {
            id: playlist

            isMusic: false

            onCurrentIndexChanged: _updateHistoryPlaylist(playlist)
            onParentIdChanged    : _updateHistoryPlaylist(playlist)
            onNameChanged        : _updateHistoryPlaylist(playlist)
        }
    }
}
