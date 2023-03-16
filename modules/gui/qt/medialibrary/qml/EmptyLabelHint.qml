// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *****************************************************************************/

import QtQuick 2.11
import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets

EmptyLabel {
    // Aliases

    property alias hint: labelHint.text

    property alias labelHint: labelHint

    // Settings

    spacing: VLCStyle.margin_normal

    // Children

    Widgets.CaptionLabel {
        id: labelHint

        width: parent.width

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        wrapMode: Text.WordWrap
    }
}
