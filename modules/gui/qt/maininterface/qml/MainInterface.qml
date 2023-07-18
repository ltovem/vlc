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
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

import org.videolan.vlc 0.1
import org.videolan.compat 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"

import "qrc:///playlist/" as PL

Item {
    id: g_mainInterface

    property bool _interfaceReady: false
    property bool _playlistReady: false

    BindingCompat {
        target: VLCStyle
        property: "appWidth"
        value: g_mainInterface.width
    }

    BindingCompat {
        target: VLCStyle
        property: "appHeight"
        value: g_mainInterface.height
    }

    Window.onWindowChanged: {
        if (Window.window && !Qt.colorEqual(Window.window.color, "transparent")) {
            Window.window.color = Qt.binding(function() { return theme.bg.primary })
        }
    }

    ColorContext {
        id: theme
        palette: VLCStyle.palette
        colorSet: ColorContext.View
    }

    Widgets.ToolTipExt {
        id: attachedToolTip

        parent: null
        z: 99
        colorContext.palette: parent && parent.colorContext ? parent.colorContext.palette
                                                            : VLCStyle.palette

        Component.onCompleted: {
            MainCtx.setAttachedToolTip(this)
        }
    }

    Loader {
        id: playlistWindowLoader
        asynchronous: true
        active: !MainCtx.playlistDocked && MainCtx.playlistVisible
        source: "qrc:///playlist/PlaylistDetachedWindow.qml"
    }
    Connections {
        target: playlistWindowLoader.item
        onClosing: MainCtx.playlistVisible = false
    }

    Connections {
        target: MainPlaylistController

        onPlaylistInitialized: {
            g_mainInterface._playlistReady = true
            if (g_mainInterface._interfaceReady)
                setInitialView()
        }
    }

    readonly property var pageModel: [
        { name: "mc", url: "qrc:///main/MainDisplay.qml" },
        { name: "player", url:"qrc:///player/Player.qml" },
    ]

    function loadCurrentHistoryView() {
        const current = History.current
        if ( !current || !current.name  || !current.properties ) {
            console.warn("unable to load requested view, undefined")
            return
        }
        stackView.loadView(g_mainInterface.pageModel, current.name, current.properties)

        MainCtx.mediaLibraryVisible = (current.name !== "player")
    }

    Connections {
        target: History
        onCurrentChanged: loadCurrentHistoryView()
    }

    Connections {
        target: MainCtx

        onMediaLibraryVisibleChanged: {
            if (MainCtx.mediaLibraryVisible) {
                // NOTE: Useful when we started the application on the 'player' view.
                if (History.previousEmpty) {
                    if (MainCtx.hasEmbededVideo && MainCtx.canShowVideoPIP === false)
                        MainPlaylistController.stop()

                    _pushHome()

                    return
                }

                if (History.current.name !== "player")
                    return

                if (MainCtx.hasEmbededVideo && MainCtx.canShowVideoPIP === false)
                    MainPlaylistController.stop()

                History.previous()
            } else {
                if (History.current.name === "player")
                    return

                stackView.currentItem._inhibitMiniPlayer = true

                History.push(["player"])
            }
        }
    }

    function setInitialView() {
        //set the initial view
        const loadPlayer = !MainPlaylistController.empty;

        if (MainCtx.mediaLibraryAvailable)
            History.push(["mc", "video"],
                         Qt.OtherFocusReason, loadPlayer ? History.Stay : History.Go)
        else
            History.push(["mc", "home"],
                         Qt.OtherFocusReason, loadPlayer ? History.Stay : History.Go)

        if (loadPlayer)
            History.push(["player"])
    }

    function _pushHome() {
        if (MainCtx.mediaLibraryAvailable)
            History.push(["mc", "video"])
        else
            History.push(["mc", "home"])
    }

    Component.onCompleted: {
        g_mainInterface._interfaceReady = true;
        if (g_mainInterface._playlistReady)
            setInitialView()
    }


    DropArea {
        anchors.fill: parent
        onDropped: {
            let urls = []
            if (drop.hasUrls) {

                for (let i = 0; i < drop.urls.length; i++)
                    urls.push(drop.urls[i])

            } else if (drop.hasText) {
                /* Browsers give content as text if you dnd the addressbar,
                   so check if mimedata has valid url in text and use it
                   if we didn't get any normal Urls()*/

                urls.push(drop.text)
            }

            if (urls.length > 0) {
                /* D&D of a subtitles file, add it on the fly */
                if (MainPlayerController.isPlaying && urls.length == 1) {
                    if (MainPlayerController.associateSubtitleFile(urls[0])) {
                        drop.accept()
                        return
                    }
                }

                MainPlaylistController.append(urls, true)
                drop.accept()
            }
        }

        Widgets.StackViewExt {
            id: stackView
            anchors.fill: parent
            focus: true

            Connections {
                target: MainPlayerController
                onPlayingStateChanged: {
                    if (MainPlayerController.playingState === MainPlayerController.PLAYING_STATE_STOPPED
                            && History.current.name === "player") {
                        if (History.previousEmpty)
                            _pushHome()
                        else
                            History.previous()
                    }
                }
            }
        }
    }

    Loader {
        asynchronous: true
        source: "qrc:///menus/GlobalShortcuts.qml"
    }

    MouseArea {
        /// handles mouse navigation buttons
        anchors.fill: parent
        acceptedButtons: Qt.BackButton
        cursorShape: undefined
        onClicked: History.previous()
    }

    Loader {
        active: {
            const windowVisibility = MainCtx.intfMainWindow.visibility
            return MainCtx.clientSideDecoration
                    && (windowVisibility !== Window.Maximized)
                    && (windowVisibility !== Window.FullScreen)

        }

        source: "qrc:///widgets/CSDMouseStealer.qml"
    }

}
