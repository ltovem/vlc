/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick.Controls 2.4
import QtQuick 2.11
import QtQml.Models 2.2
import QtQuick.Layouts 1.11

import org.videolan.medialib 0.1
import org.videolan.vlc 0.1

import "qrc:///util/" as Util
import "qrc:///widgets/" as Widgets
import "qrc:///main/" as MainInterface
import "qrc:///style/"


Widgets.PageLoader {
    id: root

    property MLModel model

    pageModel: [{
        name: "all",
        component: allArtistsComponent
    }, {
        name: "albums",
        component: artistAlbumsComponent
    }]

    loadDefaultView: function () {
        History.update(["mc", "music", "artists", "all"])
        loadPage("all")
    }

    onCurrentItemChanged: {
        model = currentItem.model
    }

    function _updateArtistsAllHistory(currentIndex) {
        History.update(["mc", "music", "artists", "all", { "initialIndex": currentIndex }])
    }

    function _updateArtistsAlbumsHistory(currentIndex, initialAlbumIndex) {
        History.update(["mc","music", "artists", "albums", {
            "initialIndex": currentIndex,
            "initialAlbumIndex": initialAlbumIndex,
        }])
    }

    Component {
        id: allArtistsComponent

        MusicAllArtists {
            onCurrentIndexChanged: _updateArtistsAllHistory(currentIndex)

            onRequestArtistAlbumView: {
                History.push(["mc", "music", "artists", "albums",
                              { initialIndex: currentIndex } ]);

                stackView.currentItem.setCurrentItemFocus(reason);
            }
        }
    }

    Component {
        id: artistAlbumsComponent

        MusicArtistsAlbums {

            Navigation.parentItem: root

            onCurrentIndexChanged: _updateArtistsAlbumsHistory(currentIndex, currentAlbumIndex)
            onCurrentAlbumIndexChanged: _updateArtistsAlbumsHistory(currentIndex, currentAlbumIndex)
        }
    }
}
