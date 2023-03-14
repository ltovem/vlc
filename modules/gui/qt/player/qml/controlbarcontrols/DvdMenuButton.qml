/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

import org.videolan.vlc 0.1

Widgets.IconControlButton {

    enabled: Player.hasMenu

    iconText: VLCIcons.dvd_menu
    text: I18n.qtr("Menu")

    onClicked: Player.sectionMenu()
}
