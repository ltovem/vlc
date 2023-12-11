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

    property int horizontalSpacing: _defaultHorizontalSpacing
    readonly property int _defaultHorizontalSpacing: VLCStyle.column_spacing

    property int maxNbItemPerRow
    readonly property int nbItemPerRow: {
        let _nbItemPerRow = Math.max(
            Math.floor(
                (availableWidth + _defaultHorizontalSpacing) /
                (basePictureWidth + _defaultHorizontalSpacing)
            ), 1
        )

        return maxNbItemPerRow ? Math.min(_nbItemPerRow, maxNbItemPerRow) : _nbItemPerRow
    }

    onNbItemPerRowChanged: {
        if(nbItemPerRow === 2)
            horizontalSpacing = VLCStyle.margin_large + VLCStyle.margin_xxxsmall
        else if(nbItemPerRow === 3)
            horizontalSpacing = VLCStyle.margin_large + VLCStyle.margin_xxsmall
        else
            horizontalSpacing = _defaultHorizontalSpacing
    }

    // NOTE: Responsive cell sizing based on available width
    readonly property int cellWidth: (availableWidth + horizontalSpacing) / nbItemPerRow - horizontalSpacing
    readonly property int cellHeight: (basePictureHeight / basePictureWidth) * cellWidth + textHeight

    //NOTE: These factors have been selected because they downscale the thumbnails in most of the cases,
    //      for a balance of performance/quality
    property real pictureWidthResizeFactor: {
        if(maxNbItemPerRow === 6)
            return 2.2
        else
            return 2.7
    }

    property real appWidthFactor:{
        if (VLCStyle.appWidth <= 1920)
            return 0.5
        else if (VLCStyle.appWidth <= 2560)
            return 0.7
        else
            return 1
    }

    // NOTE: Find the maximum picture sizes until nbItemPerRow changes,
    //       so that we can downscale the thumbnails
    readonly property int maxPictureWidth: nbItemPerRow == maxNbItemPerRow ? Math.floor(basePictureWidth *
                                                                                        pictureWidthResizeFactor * appWidthFactor):
                                               (basePictureWidth + horizontalSpacing) * (1 + 1 / nbItemPerRow) - horizontalSpacing
    readonly property int maxPictureHeight: (basePictureHeight / basePictureWidth) * maxPictureWidth
}
