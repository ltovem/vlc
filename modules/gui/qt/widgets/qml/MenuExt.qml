/*****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
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
import QtQuick.Controls 2.12

Menu {
    contentItem.focus: true

    delegate: MenuItem { height: (text.length > 0) ? implicitHeight : 0 }

    function _addMenu(menu /* : QtObject */) {
        console.assert(menu instanceof Menu)
        addMenu(menu)
    }

    function _addAction(action /* : QtObject */) {
        console.assert(action instanceof Action)
        addAction(action)
    }

    function _removeAction(action /* : QtObject */) {
        console.assert(action instanceof Action)
        removeAction(action)
    }

    function _insertAction(index /* : int */, action /* : QtQbject */) {
        console.assert(action instanceof Action)
        insertAction(index, action)
    }
}
