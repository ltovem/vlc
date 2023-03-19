// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets


FocusScope {
    id: playerControlLayout

    implicitWidth: layoutLoader_left.implicitWidth + layoutLoader_center.implicitWidth + layoutLoader_right.implicitWidth + 2 * layoutSpacing
    implicitHeight: VLCStyle.maxControlbarControlHeight

    property real defaultSize: VLCStyle.icon_normal // default size for IconToolButton based controls

    property real spacing: VLCStyle.margin_normal // spacing between controls
    property real layoutSpacing: VLCStyle.margin_xxlarge // spacing between layouts (left, center, and right)

    property int identifier: -1
    readonly property PlayerControlbarModel model: {
        if (!!MainCtx.controlbarProfileModel.currentModel)
            MainCtx.controlbarProfileModel.currentModel.getModel(identifier)
        else
            null
    }

    signal requestLockUnlockAutoHide(bool lock)

    Component.onCompleted: {
        console.assert(identifier >= 0)
    }

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Window
    }

    Loader {
        id: layoutLoader_left

        anchors {
            right: layoutLoader_center.left
            left: parent.left
            top: parent.top
            bottom: parent.bottom

            // Spacing for the filler item acts as padding
            rightMargin: layoutSpacing - spacing
        }

        active: !!playerControlLayout.model
                && !!playerControlLayout.model.left

        focus: true

        sourceComponent: ControlLayout {
            model: ControlListFilter {
                sourceModel: playerControlLayout.model.left

                player: Player
                ctx: MainCtx
            }

            Navigation.parentItem: playerControlLayout
            Navigation.rightItem: layoutLoader_center.item

            focus: true

            altFocusAction: Navigation.defaultNavigationRight

            onRequestLockUnlockAutoHide: playerControlLayout.requestLockUnlockAutoHide(lock)
        }
    }

    Loader {
        id: layoutLoader_center

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            bottom: parent.bottom
        }

        active: !!playerControlLayout.model
                && !!playerControlLayout.model.center

        width: (parent.width < implicitWidth) ? parent.width
                                              : implicitWidth

        sourceComponent: ControlLayout {
            model: ControlListFilter {
                sourceModel: playerControlLayout.model.center

                player: Player
                ctx: MainCtx
            }

            Navigation.parentItem: playerControlLayout
            Navigation.leftItem: layoutLoader_left.item
            Navigation.rightItem: layoutLoader_right.item

            focus: true

            altFocusAction: Navigation.defaultNavigationUp

            onRequestLockUnlockAutoHide: playerControlLayout.requestLockUnlockAutoHide(lock)
        }
    }

    Loader {
        id: layoutLoader_right

        anchors {
            left: layoutLoader_center.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom

            // Spacing for the filler item acts as padding
            leftMargin: layoutSpacing - spacing
        }

        active: !!playerControlLayout.model
                && !!playerControlLayout.model.right

        sourceComponent: ControlLayout {
            model: ControlListFilter {
                sourceModel: playerControlLayout.model.right

                player: Player
                ctx: MainCtx
            }

            rightAligned: true

            Navigation.parentItem: playerControlLayout
            Navigation.leftItem: layoutLoader_center.item

            focus: true

            altFocusAction: Navigation.defaultNavigationLeft

            onRequestLockUnlockAutoHide: playerControlLayout.requestLockUnlockAutoHide(lock)
        }
    }
}
