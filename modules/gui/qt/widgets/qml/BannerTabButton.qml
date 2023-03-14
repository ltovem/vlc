/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Templates 2.4 as T
import QtQuick.Layouts 1.11

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

T.TabButton {
    id: control

    // Properties

    property bool selected: false

    property string iconTxt: ""

    property bool showText: true
    property bool showCurrentIndicator: true

    // Settings

    width: control.showText ? VLCStyle.bannerTabButton_width_large
                            : VLCStyle.icon_banner

    height: implicitHeight

    implicitWidth: contentItem.implicitWidth
    implicitHeight: contentItem.implicitHeight

    padding: 0

    text: model.displayText

    // Keys

    Keys.priority: Keys.AfterItem

    Keys.onPressed: Navigation.defaultKeyAction(event)

    // Childs

    ColorContext {
        id: theme
        colorSet: ColorContext.TabButton

        focused: control.activeFocus
        hovered: control.hovered
        pressed: control.down
        enabled: control.enabled
    }

    background: Widgets.AnimatedBackground {
        height: control.height
        width: control.width

        active: visualFocus
        animate: theme.initialized

        animationDuration: VLCStyle.duration_short

        backgroundColor: theme.bg.primary
        foregroundColor: control.selected ? theme.fg.secondary : theme.fg.primary
        activeBorderColor: theme.visualFocus
    }

    contentItem: Item {
        implicitWidth: tabRow.implicitWidth + VLCStyle.margin_xxsmall * 2
        implicitHeight: tabRow.implicitHeight

        RowLayout {
            id: tabRow

            anchors.centerIn: parent
            anchors.leftMargin: VLCStyle.margin_xxsmall
            anchors.rightMargin: VLCStyle.margin_xxsmall

            spacing: VLCStyle.margin_xsmall

            Widgets.IconLabel {
                text: control.iconTxt

                color: (control.selected || control.activeFocus || control.hovered)
                        ? theme.accent
                        : theme.fg.primary

                font.pixelSize: VLCStyle.icon_banner
            }

            T.Label {
                visible: showText

                text: control.text

                color: control.background.foregroundColor

                font.pixelSize: VLCStyle.fontSize_normal

                font.weight: (control.activeFocus ||
                              control.hovered     ||
                              control.selected) ? Font.DemiBold
                                                : Font.Normal
            }
        }

        Widgets.CurrentIndicator {
            width: tabRow.width

            orientation: Qt.Horizontal

            margin: VLCStyle.margin_xxsmall

            visible: (control.showCurrentIndicator && control.selected)
        }
    }
}
