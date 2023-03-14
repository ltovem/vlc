/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Templates 2.4 as T
import "qrc:///style/"

T.Label {
    elide: Text.ElideRight
    font.pixelSize: VLCStyle.fontSize_xxxlarge
    font.weight: Font.Bold
    textFormat: Text.PlainText
}
