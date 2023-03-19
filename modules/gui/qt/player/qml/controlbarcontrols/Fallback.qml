// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"
import "qrc:///util/Helpers.js" as Helpers


Control {
    padding: VLCStyle.focus_border

    Keys.priority: Keys.AfterItem
    Keys.onPressed: Navigation.defaultKeyAction(event)

    property bool paintOnly: false

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.View
    }

    background: Widgets.AnimatedBackground {
        active: visualFocus
        animate: theme.initialized
        activeBorderColor: theme.visualFocus
    }

    contentItem: Widgets.MenuLabel {
        text: I18n.qtr("WIDGET\nNOT\nFOUND")
        horizontalAlignment: Text.AlignHCenter
        color: theme.fg.primary
    }
}
