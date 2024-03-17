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

import "qrc:///style/"

Item{
    // NOTE: Base picture sizing
    /* required */ property int basePictureWidth
    /* required */ property int basePictureHeight

    /* required */ property int availableWidth

    property int titleTopMargin: VLCStyle.gridItemTitle_topMargin
    property int titleHeight: VLCStyle.gridItemTitle_height

    property int subtitleTopMargin: VLCStyle.gridItemSubtitle_topMargin
    property int subtitleHeight: VLCStyle.gridItemSubtitle_height

    // NOTE: The total text height of title and subtitle
    //       1px is added to each, to correctly represent the actual height
    readonly property int textHeight: (titleHeight ? titleHeight + titleTopMargin + VLCStyle.margin_xxxsmall / 2: 0) +
                                      (subtitleHeight ? subtitleHeight + subtitleTopMargin + VLCStyle.margin_xxxsmall / 2 : 0)

    property int horizontalSpacing: VLCStyle.column_spacing

    readonly property int nbItemPerRow: Math.max(Math.floor((availableWidth + horizontalSpacing) /
                                                            (basePictureWidth + horizontalSpacing)), 1)

    // NOTE: Responsive cell sizing based on available width
    readonly property int cellWidth: (availableWidth + horizontalSpacing) / nbItemPerRow - horizontalSpacing
    readonly property int cellHeight: (basePictureHeight / basePictureWidth) * cellWidth + textHeight

    // NOTE: Find the maximum picture sizes until nbItemPerRow changes,
    //       so that we can downscale the thumbnails
    readonly property int maxPictureWidth: (basePictureWidth + horizontalSpacing) * (1 + 1 / nbItemPerRow) - horizontalSpacing
    readonly property int maxPictureHeight: (basePictureHeight / basePictureWidth) * maxPictureWidth
}
