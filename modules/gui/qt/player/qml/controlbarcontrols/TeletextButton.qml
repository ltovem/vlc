/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Controls 2.4

import org.videolan.vlc 0.1

import "qrc:///style/"

ControlButtonPopup {
    id: root

    // Settings

    enabled: Player.isTeletextAvailable

    iconText: VLCIcons.tvtelx

    text: I18n.qtr("Teletext")

    popupContent: TeletextWidget {
        colorContext.palette: root.colorContext.palette

        Navigation.parentItem: root
        Navigation.downItem: root
    }
}
