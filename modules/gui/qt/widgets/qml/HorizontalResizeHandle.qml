/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import "qrc:///style/"
import org.videolan.vlc 0.1

// targetWidth: concerned widget's current width
// sourceWidth: target's width is bounded by this value (parent's width?)
// HorizontalResizeHandle actually doesn't resizes target
// you have to assign target's width manually using widthFactor property
// the idea behind using widthFactor is to maintain scale ratio when source itself resizes
// e.g target.width: Helpers.clamp(sourceWidth / resizeHandle.widthFactor, minimumWidth, maximumWidth)
MouseArea {
    id: root

    // provided by parent, this widget don't modify these properties
    property int sourceWidth
    property int targetWidth

    property double widthFactor: 4
    property bool atRight: true

    property int _previousX

    cursorShape: Qt.SplitHCursor
    width: VLCStyle.dp(8, VLCStyle.scale)
    acceptedButtons: Qt.LeftButton

    onPressed: {
        MainCtx.setCursor(cursorShape)
        _previousX = mouseX
    }

    onReleased: {
        MainCtx.restoreCursor()
    }

    onPositionChanged: {
        var f = atRight ? -1 : 1
        var delta = mouseX - _previousX

        root.widthFactor = root.sourceWidth / (root.targetWidth + (delta * - f))
    }

}
