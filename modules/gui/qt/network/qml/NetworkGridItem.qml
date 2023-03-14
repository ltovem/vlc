/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQml.Models 2.2
import QtGraphicalEffects 1.0

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Widgets.GridItem {
    id: root

    property var model: ({})
    property int index: -1

    width: VLCStyle.gridItem_network_width
    height: VLCStyle.gridItem_network_height

    pictureWidth: VLCStyle.gridCover_network_width
    pictureHeight: VLCStyle.gridCover_network_height

    playCoverBorderWidth: VLCStyle.gridCover_network_border

    playCoverShowPlay: (model.type !== NetworkMediaModel.TYPE_NODE
                        &&
                        model.type !== NetworkMediaModel.TYPE_DIRECTORY)

    image: {
        if (model.artwork && model.artwork.toString() !== "") {
            return model.artwork
        } else {
            var f = function(type) {
                switch (type) {
                case NetworkMediaModel.TYPE_DISC:
                    return "qrc://sd/disc.svg"
                case NetworkMediaModel.TYPE_CARD:
                    return "qrc://sd/capture-card.svg"
                case NetworkMediaModel.TYPE_STREAM:
                    return "qrc://sd/stream.svg"
                case NetworkMediaModel.TYPE_PLAYLIST:
                    return "qrc://sd/playlist.svg"
                case NetworkMediaModel.TYPE_FILE:
                    return "qrc://sd/file.svg"
                default:
                    return "qrc://sd/directory.svg"
                }
            }
            return SVGColorImage.colorize(f(model.type))
                                .color1(root.colorContext.fg.primary)
                                .accent(root.colorContext.accent)
                                .uri()
        }
    }

    title: model.name || I18n.qtr("Unknown share")
    subtitle: {
       if (!model.mrl) {
         return ""
       } else if ((model.type === NetworkMediaModel.TYPE_NODE || model.type === NetworkMediaModel.TYPE_DIRECTORY) && model.mrl.toString() === "vlc://nop") {
         return ""
      } else {
         return model.mrl
      }
    }
}
