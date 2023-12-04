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

import org.videolan.vlc 0.1

SortFilterProxyModel {
    filterAcceptsRowFunction: function(source, index) {
        const id = sourceModel.data(sourceModel.index(source, 0), ControlListModel.ID_ROLE)
        if (id === undefined)
            return true

        const item = conditionMap.find((i) => { return i.id === id })
        if (item === undefined)
            return true

        return item.condition
    }

    readonly property var conditionMap: [
        {id: ControlListModel.NAVIGATION_BUTTONS, condition: (Player.hasMenu || Player.hasPrograms || Player.isTeletextAvailable)},
        {id: ControlListModel.BOOKMARK_BUTTON, condition: (MainCtx.mediaLibraryAvailable || Player.hasChapters || Player.hasTitles)},
        {id: ControlListModel.PROGRAM_BUTTON, condition: (Player.hasPrograms)},
        {id: ControlListModel.TELETEXT_BUTTONS, condition: (Player.isTeletextAvailable)}
    ]

    onConditionMapChanged: invalidateFilter()
}
