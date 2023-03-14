/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick          2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts  1.11
import QtQml.Models     2.2

import org.videolan.vlc 0.1
import org.videolan.medialib 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

Widgets.PageLoader {
    id: root

    // Properties

    property var model
    property var sortMenu
    property var sortModel

    // Settings

    pageModel: [{
        name: "base",
        component: componentBase
    }, {
        name: "group",
        component: componentGroup
    }]

    loadDefaultView: function () {
        History.update(["mc", "video", "all", "base"])
        loadPage("base")
    }

    // Events

    onCurrentItemChanged: {
        model     = currentItem.model
        sortMenu  = currentItem.sortMenu
        sortModel = currentItem.sortModel
    }
    // Functions private

    function _updateHistoryAll(index) {
        History.update(["mc", "video", "all", "base", { "initialIndex": index }])
    }

    function _updateHistoryGroup(group) {
        History.update(["mc", "video", "all", "group", {
                            "initialIndex": group.currentIndex,
                            "parentId" : group.parentId,
                            "title" : group.title
                        }])
    }

    // Children

    Component {
        id: componentBase

        VideoAllSubDisplay {
            // Events

            onShowList: {
                History.push(["mc", "video", "all", "group",
                             { parentId: model.id, title: model.title }])

                root.stackView.currentItem.setCurrentItemFocus(reason)
            }

            // NOTE: The model can change over time.
            onModelChanged: root.model = model

            onCurrentIndexChanged: root._updateHistoryAll(currentIndex)
        }
    }

    Component {
        id: componentGroup

        MediaGroupDisplay {
            id: group

            anchors.fill: parent

            onCurrentIndexChanged: root._updateHistoryGroup(group)
            onParentIdChanged    : root._updateHistoryGroup(group)
            onTitleChanged       : root._updateHistoryGroup(group)

            function isInfoExpandPanelAvailable(/* modelIndexData */) {
                return true
            }
        }
    }
}
