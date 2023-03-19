// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4

import org.videolan.vlc 0.1

import "qrc:///style/"

Label {
    font.pixelSize: VLCStyle.icon_medium
    font.family: VLCIcons.fontFamily

    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
}
