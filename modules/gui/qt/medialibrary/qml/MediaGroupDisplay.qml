/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11

import org.videolan.medialib 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///util/" as Util
import "qrc:///style/"

VideoAll {
    id: root

    // Properties

    // Aliases

    // NOTE: This is used to determine which media(s) shall be displayed.
    property alias parentId: modelVideo.parentId

    // NOTE: The title of the group.
    property string title: ""

    // Children

    model: MLVideoModel {
        id: modelVideo

        ml: MediaLib

        parentId: initialId
    }

    contextMenu: Util.MLContextMenu { model: modelVideo; showPlayAsAudioAction: true }

    header: Widgets.SubtitleLabel {
        width: root.width

        // NOTE: We want this to be properly aligned with the grid items.
        leftPadding: root.contentMargin

        bottomPadding: VLCStyle.margin_normal

        text: root.title
        color: root.colorContext.fg.primary
    }
}
