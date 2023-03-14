/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1
import org.videolan.medialib 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Widgets.GridItem {
    property var model: ({})
    property int index: -1

    image: model.cover || VLCStyle.noArtAlbumCover
    title: model.title || I18n.qtr("Unknown title")
    subtitle: model.main_artist || I18n.qtr("Unknown artist")
    pictureWidth: VLCStyle.gridCover_music_width
    pictureHeight: VLCStyle.gridCover_music_height
    playCoverBorderWidth: VLCStyle.gridCover_music_border
    onPlayClicked: {
        if ( model.id !== undefined ) {
            MediaLib.addAndPlay( model.id )
        }
    }
}
