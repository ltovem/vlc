// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Widgets.ScaledImage {
    id: custom_cover

    property var networkModel
    property color bgColor
    property color color1
    property color accent

    sourceSize: Qt.size(width, height)
    source: {
        if (networkModel === null)
            return ""

        if (!!networkModel.artwork && networkModel.artwork.length > 0)
            return networkModel.artwork

        var img = SVGColorImage.colorize(_baseUri(networkModel.type))
            .color1(custom_cover.color1)
            .accent(custom_cover.accent)

        if (bgColor !== undefined)
            img.background(custom_cover.bgColor)

        return img.uri()
    }

    function _baseUri(type) {
        switch (type) {
        case NetworkMediaModel.TYPE_DISC:
            return "qrc:///sd/disc.svg"
        case NetworkMediaModel.TYPE_CARD:
            return "qrc:///sd/capture-card.svg"
        case NetworkMediaModel.TYPE_STREAM:
            return "qrc:///sd/stream.svg"
        case NetworkMediaModel.TYPE_PLAYLIST:
            return "qrc:///sd/playlist.svg"
        case NetworkMediaModel.TYPE_FILE:
            return "qrc:///sd/file.svg"
        default:
            return "qrc:///sd/directory.svg"
        }
    }
}
