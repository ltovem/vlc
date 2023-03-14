/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11

import org.videolan.vlc 0.1
import org.videolan.medialib 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///util/" as Util
import "qrc:///style/"

VideoAll {
    id: root

    // Properties

    // NOTE: We are exposing a custom SortMenu with grouping options.
    property SortMenuVideo sortMenu: SortMenuVideo {
        ctx: MainCtx

        onGrouping: MainCtx.grouping = grouping
    }

    // Private

    property var _meta: null

    // Signals

    signal showList(var model, int reason)

    // Settings

    anchors.fill: parent

    model: !!_meta ? _meta.model : null

    contextMenu: Util.MLContextMenu { model: _meta ? _meta.model : null; showPlayAsAudioAction: true }

    // Functions

    function _updateMetaModel(groupping) {
        if (root._meta)
            root._meta.destroy()

        if (groupping === MainCtx.GROUPING_NAME) {
            root._meta = groupComponent.createObject(root)
        } else if (groupping === MainCtx.GROUPING_FOLDER) {
            root._meta = folderComponent.createObject(root)
        } else {
            root._meta = videoComponent.createObject(root)
        }
    }

    function getLabelGroup(model, string) {
        if (!model) return ""

        var count = model.count

        if (count === 1) {
            return getLabel(model)
        } else {
            if (count < 100)
                return [ string.arg(count) ]
            else
                return [ string.arg("99+") ]
        }
    }

    // VideoAll reimplementation

    function setCurrentItemFocus(reason) {
        if (headerItem && headerItem.focus)
            headerItem.forceActiveFocus(reason)
        else
            _currentView.setCurrentItemFocus(reason)
    }

    // VideoAll events reimplementation

    function onAction(indexes) { _meta.onAction(indexes) }

    function onDoubleClick(object) { _meta.onDoubleClick(object) }

    function onLabelGrid(object) { return _meta.onLabelGrid(object) }
    function onLabelList(object) { return _meta.onLabelList(object) }

    function isInfoExpandPanelAvailable(modelIndexData) {
        return _meta.isInfoExpandPanelAvailable(modelIndexData)
    }

    // Children

    Connections {
        target: MainCtx
        onGroupingChanged: root._updateMetaModel(MainCtx.grouping)
    }

    Component.onCompleted: root._updateMetaModel(MainCtx.grouping)

    Component {
        id: videoComponent

        QtObject {
            id: metaVideo

            property var model: MLVideoModel { ml: MediaLib }

            function onAction(indexes) {
                MediaLib.addAndPlay(model.getIdsForIndexes(indexes))
                g_mainDisplay.showPlayer()
            }

            function onDoubleClick(object) { g_mainDisplay.play(MediaLib, object.id) }

            function onLabelGrid(object) { return root.getLabel(object) }
            function onLabelList(object) { return root.getLabel(object) }

            function isInfoExpandPanelAvailable(modelIndexData) { return true }
        }
    }

    Component {
        id: groupComponent

        QtObject {
            id: metaGroup

            property var model: MLVideoGroupsModel { ml: MediaLib }

            function onAction(indexes) {
                var index = indexes[0]

                var object = model.getDataAt(index);

                if (object.isVideo) {
                    MediaLib.addAndPlay(model.getIdsForIndexes(indexes))
                    g_mainDisplay.showPlayer()

                    return
                }

                root.showList(object, Qt.TabFocusReason)
            }

            function onDoubleClick(object) {
                if (object.isVideo) {
                    g_mainDisplay.play(MediaLib, object.id)

                    return
                }

                root.showList(object, Qt.MouseFocusReason)
            }

            function onLabelGrid(object) {
                return root.getLabelGroup(object, I18n.qtr("%1 Videos"))
            }

            function onLabelList(object) {
                return root.getLabelGroup(object, I18n.qtr("%1"))
            }

            function isInfoExpandPanelAvailable(modelIndexData) {
                return modelIndexData.isVideo
            }
        }
    }

    Component {
        id: folderComponent

        QtObject {
            id: metaFolder

            property var model: MLVideoFoldersModel { ml: MediaLib }

            function onAction(indexes) {
                var index = indexes[0]

                root.showList(model.getDataAt(index), Qt.TabFocusReason)
            }

            function onDoubleClick(object) {
                root.showList(object, Qt.MouseFocusReason)
            }

            function onLabelGrid(object) {
                return root.getLabelGroup(object, I18n.qtr("%1 Videos"))
            }

            function onLabelList(object) {
                return root.getLabelGroup(object, I18n.qtr("%1"))
            }

            function isInfoExpandPanelAvailable(modelIndexData) {
                return false
            }
        }
    }

    header: VideoDisplayRecentVideos {
        width: root.width - displayMarginBeginning - displayMarginEnd

        x: displayMarginBeginning

        // spacing between header and content
        bottomPadding: VLCStyle.margin_normal

        subtitleText: (root.model && root.model.count > 0) ? I18n.qtr("Videos") : ""

        // NOTE: We want grid items to be visible on the sides.
        displayMarginBeginning: root.contentMargin
        displayMarginEnd: displayMarginBeginning

        Navigation.parentItem: root

        Navigation.downAction: function() {
            _currentView.setCurrentItemFocus(Qt.TabFocusReason);
        }

        onImplicitHeightChanged: {
            // implicitHeight depends on underlying ml model initialization
            // and may update after view did resetFocus on initialization which
            // will break resetFocus's scrolling (because header height changed)
            // try to reapply reset focus here (ref #27071)
            if (root.currentIndex <= 0 || root.currentIndex === root.initialIndex)
                root.resetFocus()
        }
    }
}
