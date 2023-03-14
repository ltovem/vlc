
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Templates 2.4 as T

import org.videolan.vlc 0.1
import "qrc:///style/"
import "qrc:///widgets/" as Widgets

T.AbstractButton {
    id: button

    // Properties

    property bool onlyIcon: true

    property bool highlighted: false

    property color foregroundColor: theme.fg.primary
    property color backgroundColor: theme.bg.primary

    // Settings

    implicitWidth: Math.max(background.implicitWidth,
            (contentItem ? contentItem.implicitWidth : 0) + leftPadding + rightPadding)
    implicitHeight: Math.max(background.implicitHeight,
            (contentItem ? contentItem.implicitHeight : 0) + topPadding + bottomPadding)
    baselineOffset: contentItem ? contentItem.y + contentItem.baselineOffset : 0


    padding: VLCStyle.margin_xxsmall

    font.pixelSize: (onlyIcon) ? VLCStyle.icon_normal
                               : VLCStyle.fontSize_large

    // Children

    readonly property ColorContext colorContext: ColorContext {
        id: theme

        colorSet: ColorContext.ButtonStandard

        enabled: button.enabled
        focused: button.visualFocus
        hovered: button.hovered
        pressed: button.down
    }


    background: Widgets.AnimatedBackground {
        active: visualFocus

        animate: theme.initialized
        backgroundColor: button.backgroundColor
        foregroundColor: button.foregroundColor
        activeBorderColor: theme.visualFocus
    }

    contentItem: contentLoader.item

    Loader {
        id: contentLoader

        sourceComponent: (onlyIcon) ? iconTextContent
                                    : textContent
    }

    Component {
        id: iconTextContent

        Widgets.IconLabel {
            verticalAlignment: Text.AlignVCenter

            text: button.text

            elide: Text.ElideRight

            color: button.foregroundColor

            font.pixelSize: button.font.pixelSize
        }
    }

    Component {
        id: textContent

        T.Label {
            verticalAlignment: Text.AlignVCenter

            text: button.text

            elide: Text.ElideRight

            color: button.foregroundColor

            font.pixelSize: button.font.pixelSize

            font.weight: (highlighted) ? Font.DemiBold : Font.Normal
        }
    }
}
