/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Templates 2.4 as T

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Widgets.IconControlButton {
    // Settings

    iconText: VLCIcons.circle

    T.ToolTip.visible: (hovered || visualFocus)
}
