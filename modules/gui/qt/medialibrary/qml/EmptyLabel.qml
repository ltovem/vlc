// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Templates 2.4 as T
import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets

FocusScope {
    id: root

    // Aliases

    default property alias contents: column.data

    property alias spacing: column.spacing

    property alias cover: cover.source

    property alias coverWidth: coverContainer.width
    property alias coverHeight: coverContainer.height

    property alias text: label.text

    property alias column: column

    // Children

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.View
    }

    Column {
        id: column

        anchors.verticalCenter: parent.verticalCenter

        width: root.width

        spacing: VLCStyle.margin_small

        Item {
            width: parent.width
            height: label.y + label.height

            Item {
                id: coverContainer

                anchors.horizontalCenter: parent.horizontalCenter

                width: VLCStyle.colWidth(1)
                height: VLCStyle.colWidth(1)

                Image {
                    id: cover

                    anchors.fill: parent

                    asynchronous: true

                    fillMode: Image.PreserveAspectFit
                }

                Widgets.ListCoverShadow {
                    anchors.fill: cover
                }
            }

            T.Label {
                id: label

                anchors.top: coverContainer.bottom

                anchors.topMargin: VLCStyle.margin_large

                width: parent.width

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                focus: false

                wrapMode: Text.WordWrap

                color: theme.fg.primary

                font.pixelSize: VLCStyle.fontSize_xxlarge
                font.weight: Font.DemiBold
            }
        }
    }
}
