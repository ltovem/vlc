/*****************************************************************************
 * Copyright (C) 2024 VLC authors and VideoLAN
 *
 * Authors: Leon Vitanos <leon.vitanos@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

import QtQuick 2.12

import "qrc:///util/" as Util
import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Widgets.ExpandGridView {
    id: root

    // Properties

    // maximum picture sizing depending on nbItemPerRow
    readonly property int maxPictureWidth: gridHelper.maxPictureWidth
    readonly property int maxPictureHeight: gridHelper.maxPictureHeight

    // Aliases

    // constant base picture sizings
    /* required */ property alias basePictureWidth: gridHelper.basePictureWidth
    /* required */ property alias basePictureHeight: gridHelper.basePictureHeight

    property alias titleTopMargin: gridHelper.titleTopMargin
    property alias titleHeight: gridHelper.titleHeight

    property alias subtitleTopMargin: gridHelper.subtitleTopMargin
    property alias subtitleHeight: gridHelper.subtitleHeight

    property alias maxNbItemPerRow: gridHelper.maxNbItemPerRow

    // Settings

    displayMarginEnd: g_mainDisplay.displayMargin

    nbItemPerRow: gridHelper.nbItemPerRow

    // responsive cell sizing based on available area
    cellWidth: gridHelper.cellWidth
    cellHeight: gridHelper.cellHeight

    horizontalSpacing: gridHelper.horizontalSpacing

    // Children

    Util.GridSizeHelper {
        id: gridHelper

        availableWidth: root._availableContentWidth

        maxNbItemPerRow: basePictureWidth === basePictureHeight ? 10 : 6
    }
}
