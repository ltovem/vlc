/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Widgets.KeyNavigableTableView {
    id: root

    displayMarginEnd: g_mainDisplay.displayMargin

    enableEndFade: (g_mainDisplay.hasMiniPlayer === false)
}
