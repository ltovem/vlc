// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQml.Models 2.2

import org.videolan.vlc 0.1

import "qrc:///util" as Util
import "qrc:///widgets/" as Widgets
import "qrc:///style/"

FocusScope {
    id: root

    // Properties

    readonly property bool isViewMultiView: false

    property int leftPadding: 0
    property int rightPadding: 0

    //---------------------------------------------------------------------------------------------
    // Functions
    //---------------------------------------------------------------------------------------------

    function setCurrentItemFocus(reason) {
        searchField.forceActiveFocus(reason);
    }

    //---------------------------------------------------------------------------------------------
    // Childs
    //---------------------------------------------------------------------------------------------

    Column {
        anchors.fill: parent

        FocusScope {
            id: searchFieldContainer

            width: root.width
            height: searchField.height + VLCStyle.margin_normal * 2
            focus: true

            Navigation.parentItem:  root
            Navigation.downItem: (!!urlListDisplay.item) ? urlListDisplay.item : null

            Widgets.TextFieldExt {
                id: searchField

                focus: true
                anchors.centerIn: parent
                height: VLCStyle.dp(32, VLCStyle.scale)
                width: VLCStyle.colWidth(Math.max(VLCStyle.gridColumnsForWidth(root.width * .6), 2))
                placeholderText: I18n.qtr("Paste or write the URL here")
                selectByMouse: true

                onAccepted: {
                    if (urlListDisplay.status == Loader.Ready)
                        urlListDisplay.item.model.addAndPlay(text)
                    else
                        mainPlaylistController.append([text], true)
                }

                Keys.priority: Keys.AfterItem
                Keys.onPressed: searchFieldContainer.Navigation.defaultKeyAction(event)

                //ideally we should use Keys.onShortcutOverride but it doesn't
                //work with TextField before 5.13 see QTBUG-68711
                onActiveFocusChanged: {
                    if (activeFocus)
                        MainCtx.useGlobalShortcuts = false
                    else
                        MainCtx.useGlobalShortcuts = true
                }
            }
        }

        Loader {
            id: urlListDisplay

            width: parent.width
            height: parent.height - searchFieldContainer.height

            active: MainCtx.mediaLibraryAvailable
            source: "qrc:///medialibrary/UrlListDisplay.qml"

            onLoaded: {
                item.leftPadding = Qt.binding(function() {
                    return root.leftPadding
                })

                item.rightPadding = Qt.binding(function() {
                    return root.rightPadding
                })

                item.Navigation.upItem = searchField
                item.Navigation.parentItem =  root
            }
        }
    }
}
