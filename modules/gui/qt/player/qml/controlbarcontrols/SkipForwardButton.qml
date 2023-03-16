// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"


Widgets.IconControlButton {
    id: stepfwdBtn

    enabled: Player.seekable

    iconText: VLCIcons.skip_for
    onClicked: Player.jumpFwd()
    text: I18n.qtr("Step forward")
}
