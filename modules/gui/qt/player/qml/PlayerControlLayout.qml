/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

import QtQuick 2.12
import QtQuick.Layouts 1.12

import org.videolan.vlc 0.1
import org.videolan.compat 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets


FocusScope {
    id: playerControlLayout

    // Properties

    property real defaultSize: VLCStyle.icon_normal // default size for IconToolButton based controls

    property real spacing: VLCStyle.margin_normal // spacing between controls

    property real layoutSpacing: VLCStyle.margin_xxlarge // spacing between layouts (left, center, and right)

    property int identifier: -1

    property PlayerControlbarModel model: {
        if (!!MainCtx.controlbarProfileModel.currentModel)
            return MainCtx.controlbarProfileModel.currentModel.getModel(identifier)
        else
            return null
    }

    property QtAbstractItemModel leftModel: FilteredPlayerControlbarModel {
        sourceModel: playerControlLayout.model ? playerControlLayout.model.left : null
    }

    property QtAbstractItemModel centerModel: FilteredPlayerControlbarModel {
        sourceModel: playerControlLayout.model ? playerControlLayout.model.center : null
    }

    property QtAbstractItemModel rightModel: FilteredPlayerControlbarModel {
        sourceModel: playerControlLayout.model ? playerControlLayout.model.right : null
    }

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.Window
    }

    // Signals

    signal requestLockUnlockAutoHide(bool lock)

    signal menuOpened(var menu)

    // Settings

    implicitWidth: loaderLeftRight.active ? loaderLeftRight.implicitWidth
                                          : (loaderLeft.implicitWidth + loaderCenter.implicitWidth + loaderRight.implicitWidth)

    implicitHeight: Math.max(loaderLeft.implicitHeight, loaderCenter.implicitHeight, loaderRight.implicitHeight)

    // Events

    Component.onCompleted: console.assert(identifier >= 0)

    // Children

    Loader {
        id: loaderLeftRight

        anchors.fill: parent

        active: !loaderCenter.active &&
                playerControlLayout.model &&
                ((playerControlLayout.model.left && (playerControlLayout.model.left.count > 0)) ||
                (playerControlLayout.model.right && (playerControlLayout.model.right.count > 0)))

        focus: active

        sourceComponent: RowLayout {
            spacing: playerControlLayout.spacing

            focus: true

            // TODO: Qt >= 5.13 Use QConcatenateTablesProxyModel
            //       instead of multiple repeaters

            ControlRepeater {
                id: leftRepeater
                model: playerControlLayout.leftModel

                Navigation.parentItem: playerControlLayout
                Navigation.rightAction: function() {
                    const item = rightRepeater.itemAt(0)
                    if (item)
                        item.forceActiveFocus(Qt.TabFocusReason)
                    else
                        return false
                }

                availableWidth: loaderLeftRight.width
                availableHeight: loaderLeftRight.height
            }

            Item {
                function containsVisibleItem(repeater) {
                    for (let i = 0; i < repeater.count; ++i) {
                        const item = repeater.itemAt(i)

                        if (item && item.visible)
                            return true
                    }

                    return false
                }

                Layout.minimumWidth: (containsVisibleItem(leftRepeater) && containsVisibleItem(rightRepeater)) ? playerControlLayout.layoutSpacing
                                                                                                               : 0

                Layout.fillWidth: true
                visible: true
            }

            ControlRepeater {
                id: rightRepeater
                model: playerControlLayout.rightModel

                Navigation.parentItem: playerControlLayout
                Navigation.leftAction: function() {
                    const item = leftRepeater.itemAt(leftRepeater.count - 1)
                    if (item)
                        item.forceActiveFocus(Qt.BacktabFocusReason)
                    else
                        return false
                }

                availableWidth: loaderLeftRight.width
                availableHeight: loaderLeftRight.height
            }
        }
    }


    Loader {
        id: loaderLeft

        anchors {
            right: loaderCenter.left
            left: parent.left
            top: parent.top
            bottom: parent.bottom

            // Spacing for the filler item acts as padding
            rightMargin: layoutSpacing - spacing
        }

        // TODO: Filtered count
        active: !!playerControlLayout.model && !!playerControlLayout.model.left && (playerControlLayout.model.left.count > 0) &&
                !loaderLeftRight.active

        focus: active

        sourceComponent: ControlLayout {
            model: playerControlLayout.leftModel

            alignment: (Qt.AlignVCenter | Qt.AlignLeft)

            focus: true

            altFocusAction: Navigation.defaultNavigationRight

            Navigation.parentItem: playerControlLayout
            Navigation.rightItem: loaderCenter.item

            onRequestLockUnlockAutoHide: playerControlLayout.requestLockUnlockAutoHide(lock)

            onMenuOpened: playerControlLayout.menuOpened(menu)
        }
    }

    Loader {
        id: loaderCenter

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            bottom: parent.bottom
        }

        // TODO: Filtered count
        active: !!playerControlLayout.model && !!playerControlLayout.model.center && (playerControlLayout.model.center.count > 0)

        BindingCompat on width {
            delayed: true
            when: loaderCenter._componentCompleted
            value: {
                const item = loaderCenter.item

                const minimumWidth = (item && item.Layout.minimumWidth > 0) ? item.Layout.minimumWidth : implicitWidth
                const maximumWidth = (item && item.Layout.maximumWidth > 0) ? item.Layout.maximumWidth : implicitWidth

                if ((loaderLeft.active && (loaderLeft.width > 0)) || (loaderRight.active && (loaderRight.width > 0))) {
                    return minimumWidth
                } else {
                    return Math.min(loaderCenter.parent.width, maximumWidth)
                }
            }
        }

        property bool _componentCompleted: false

        Component.onCompleted: {
            _componentCompleted = true
        }

        sourceComponent: ControlLayout {
            model: playerControlLayout.centerModel

            focus: true

            altFocusAction: Navigation.defaultNavigationUp

            Navigation.parentItem: playerControlLayout
            Navigation.leftItem: loaderLeft.item
            Navigation.rightItem: loaderRight.item

            onRequestLockUnlockAutoHide: playerControlLayout.requestLockUnlockAutoHide(lock)

            onMenuOpened: playerControlLayout.menuOpened(menu)
        }
    }

    Loader {
        id: loaderRight

        anchors {
            left: loaderCenter.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom

            // Spacing for the filler item acts as padding
            leftMargin: layoutSpacing - spacing
        }

        // TODO: Filtered count
        active: !!playerControlLayout.model && !!playerControlLayout.model.right && (playerControlLayout.model.right.count > 0) &&
                !loaderLeftRight.active

        sourceComponent: ControlLayout {
            model: playerControlLayout.rightModel

            alignment: (Qt.AlignVCenter | Qt.AlignRight)

            focus: true

            altFocusAction: Navigation.defaultNavigationLeft

            Navigation.parentItem: playerControlLayout
            Navigation.leftItem: loaderCenter.item

            onRequestLockUnlockAutoHide: playerControlLayout.requestLockUnlockAutoHide(lock)

            onMenuOpened: playerControlLayout.menuOpened(menu)
        }
    }
}
