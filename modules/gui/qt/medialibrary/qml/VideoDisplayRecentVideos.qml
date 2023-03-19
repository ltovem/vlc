// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQml.Models 2.2

import org.videolan.medialib 0.1
import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///util/" as Util
import "qrc:///util/Helpers.js" as Helpers
import "qrc:///style/"

FocusScope {
    id: root

    // Aliases

    property alias bottomPadding: recentVideosColumn.bottomPadding

    property alias displayMarginBeginning: listView.displayMarginBeginning
    property alias displayMarginEnd: listView.displayMarginEnd

    property alias subtitleText : subtitleLabel.text

    // Settings

    implicitHeight: recentVideosColumn.height

    focus: listView.count > 0

    // Functions

    function _play(id) {
        MediaLib.addAndPlay( id, [":restore-playback-pos=2"] )
        g_mainDisplay.showPlayer()
    }

    // Childs

    Util.MLContextMenu {
        id: contextMenu

        model: listView.model

        showPlayAsAudioAction: true
    }

    readonly property ColorContext colorContext: ColorContext {
        id: theme
        colorSet: ColorContext.View
    }

    Column {
        id: recentVideosColumn

        width: root.width

        spacing: VLCStyle.margin_normal

        Widgets.SubtitleLabel {
            text: I18n.qtr("Continue Watching")

            // NOTE: Setting this to listView.visible seems to causes unnecessary implicitHeight
            //       calculations in the Column parent.
            visible: listView.count > 0
            color: theme.fg.primary
        }

        Widgets.KeyNavigableListView {
            id: listView

            width: parent.width

            implicitHeight: VLCStyle.gridItem_video_height + VLCStyle.gridItemSelectedBorder
                            +
                            VLCStyle.margin_xlarge

            spacing: VLCStyle.column_spacing

            // NOTE: We want navigation buttons to be centered on the item cover.
            buttonMargin: VLCStyle.margin_xsmall + VLCStyle.gridCover_video_height / 2 - buttonLeft.height / 2

            orientation: ListView.Horizontal

            focus: true

            // NOTE: We want a gentle fade at the beginning / end of the history.
            enableFade: true

            Navigation.parentItem: root

            visible: listView.count > 0

            model: MLRecentsVideoModel {
                ml: MediaLib
            }

            delegate: VideoGridItem {
                id: gridItem

                pictureWidth: VLCStyle.gridCover_video_width
                pictureHeight: VLCStyle.gridCover_video_height

                selected: activeFocus

                focus: true

                onItemDoubleClicked: gridItem.play()

                onItemClicked: {
                    listView.currentIndex = index
                    this.forceActiveFocus(Qt.MouseFocusReason)
                }

                // NOTE: contextMenu.popup wants a list of indexes.
                onContextMenuButtonClicked: {
                    contextMenu.popup([listView.model.index(index, 0)],
                                      globalMousePos,
                                      { "player-options": [":restore-playback-pos=2"] })
                }

                dragItem: Widgets.DragItem {
                    coverRole: "thumbnail"

                    indexes: [index]

                    onRequestData: {
                        setData(identifier, [model])
                    }

                    function getSelectedInputItem(cb) {
                        return MediaLib.mlInputItem([model.id], cb)
                    }
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: VLCStyle.duration_short
                    }
                }

                function play() {
                    if (model.id !== undefined) {
                        root._play(model.id)
                    }
                }
            }

            onActionAtIndex: {
                root._play(model.getIdForIndex(index))
            }
        }

        Widgets.SubtitleLabel {
            id: subtitleLabel

            visible: text !== ""
            color: theme.fg.primary
        }
    }
}
