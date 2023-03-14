/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11

import org.videolan.vlc 0.1
import "qrc:///style/"

ButtonExt {
    id: control

    colorContext.colorSet: ColorContext.ButtonAccent
    iconSize: VLCStyle.icon_actionButton
}
