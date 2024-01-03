/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
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
import QtQuick.Templates 2.12 as T

import org.videolan.vlc 0.1

T.Pane {
    id: root

    // Properties

    property int indexFocus: -1

    property int _countEnabled: 0

    // Aliases

    property alias count: repeater.count

    property alias model: repeater.model
    property alias delegate: repeater.delegate

    // Settings

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    Navigation.navigable: (_countEnabled > 0)

    // Events

    // Keys

    Keys.priority: Keys.AfterItem

    Keys.onPressed: root.Navigation.defaultKeyAction(event)

    // Functions

    function _hasFocus() {
        for (let i = 0 ; i < count; i++) {
            if (repeater.itemAt(i).activeFocus)
                return true;
        }

        return false;
    }

    // Childs

    Component {
        id: enabledConnection

        Connections {
            onEnabledChanged: root._countEnabled += (target.enabled ? 1 : -1)
        }
    }

    contentItem: Row {
        spacing: root.spacing

        Repeater{
            id: repeater

            onItemAdded: {
                if (index === 0)
                    item.focus = true

                if (item.enabled) root._countEnabled += 1;

                enabledConnection.createObject(item, { target: item });

                item.Navigation.parentItem = root;

                item.Navigation.leftAction = function() {
                    let i = index;

                    do {
                        i--;
                    } while (i >= 0
                             &&
                             (!repeater.itemAt(i).enabled || !repeater.itemAt(i).visible));

                    if (i == -1)
                        root.Navigation.defaultNavigationLeft();
                    else
                        repeater.itemAt(i).forceActiveFocus(Qt.BacktabFocusReason);
                };

                item.Navigation.rightAction = function() {
                    let i = index;

                    do {
                        i++;
                    } while (i < count
                             &&
                             (!repeater.itemAt(i).enabled || !repeater.itemAt(i).visible));

                    if (i === count)
                        root.Navigation.defaultNavigationRight();
                    else
                        repeater.itemAt(i).forceActiveFocus(Qt.TabFocusReason);
                };
            }

            onItemRemoved: if (item.enabled) root._countEnabled -= 1
        }
    }
}
