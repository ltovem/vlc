// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Templates 2.4 as T
import "qrc:///style/"

T.Label {
    elide: Text.ElideRight

    font {
        pixelSize: VLCStyle.icon_normal
        family: VLCIcons.fontFamily
    }
    textFormat: Text.PlainText
}
