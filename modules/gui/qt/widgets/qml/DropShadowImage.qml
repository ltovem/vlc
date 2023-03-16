// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *****************************************************************************/

import QtQuick 2.11

import org.videolan.vlc 0.1

ScaledImage {
    property var blurRadius: null
    property var color: null
    property var xOffset: null
    property var yOffset: null
    property var xRadius: null
    property var yRadius: null

    cache: true
    asynchronous: true

    fillMode: Image.Pad

    onSourceSizeChanged: {
        // Do not load the image when size is not valid:
        if (sourceSize.width > 0 && sourceSize.height > 0)
            source = Qt.binding(function() {
                return Effects.url((xRadius > 0 || yRadius > 0) ? Effects.RoundedRectDropShadow
                                                                : Effects.RectDropShadow,
                                   {"blurRadius": blurRadius,
                                    "color": color,
                                    "xOffset": xOffset,
                                    "yOffset": yOffset,
                                    "xRadius": xRadius,
                                    "yRadius": yRadius})
            })
        else
            source = ""
    }
}
