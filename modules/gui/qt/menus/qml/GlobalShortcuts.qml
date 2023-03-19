// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1

Item {

    ShortcutExt{ context: Qt.WindowShortcut; sequence:"Ctrl+O"; onActivated: DialogsProvider.simpleOpenDialog(); }
    ShortcutExt{ sequence:"Ctrl+Shift+O"; onActivated: DialogsProvider.openFileDialog(); }
    ShortcutExt{ context: Qt.WindowShortcut; sequence:"Ctrl+F"; onActivated: DialogsProvider.PLOpenDir(); }
    ShortcutExt{ sequence:"Ctrl+D"; onActivated: DialogsProvider.openDiscDialog(); }
    ShortcutExt{ sequence:"Ctrl+N"; onActivated: DialogsProvider.openNetDialog(); }
    ShortcutExt{ sequence:"Ctrl+C"; onActivated: DialogsProvider.openCaptureDialog(); }
    ShortcutExt{ sequence:"Ctrl+V"; onActivated: DialogsProvider.openUrlDialog(); }
    ShortcutExt{ context: Qt.WindowShortcut; sequence:"Ctrl+Y"; onActivated: DialogsProvider.savePlayingToPlaylist(); }
    ShortcutExt{ sequence:"Ctrl+R"; onActivated: DialogsProvider.openAndTranscodingDialogs(); }
    ShortcutExt{ sequence:"Ctrl+S"; onActivated: DialogsProvider.openAndStreamingDialogs(); }
    ShortcutExt{ sequence:"Ctrl+Q"; onActivated: DialogsProvider.quit(); }
    ShortcutExt{ sequence:"Ctrl+E"; onActivated: DialogsProvider.extendedDialog(); }
    ShortcutExt{ sequence:"Ctrl+I"; onActivated: DialogsProvider.mediaInfoDialog(); }
    ShortcutExt{ sequence:"Ctrl+J"; onActivated: DialogsProvider.mediaCodecDialog(); }
    ShortcutExt{ sequence:"Ctrl+M"; onActivated: DialogsProvider.messagesDialog(); }
    ShortcutExt{ sequence:"Ctrl+P"; onActivated: DialogsProvider.prefsDialog(); }
    ShortcutExt{ sequence:"Ctrl+T"; onActivated: DialogsProvider.gotoTimeDialog(); }

    ShortcutExt{ sequence:"Ctrl+Shift+W"; onActivated: DialogsProvider.vlmDialog(); }

    ShortcutExt{ sequence:"Ctrl+L"; onActivated: MainCtx.playlistVisible = !MainCtx.playlistVisible; }

    ShortcutExt{ sequence:"F1"; onActivated: DialogsProvider.helpDialog() }
    ShortcutExt{ sequence:"F10"; onActivated: MainCtx.toggleToolbarMenu() }
    ShortcutExt{ sequence:"F11"; onActivated: MainCtx.toggleInterfaceFullScreen() }

    Loader {
        active: MainCtx.mediaLibraryAvailable
        source: "qrc:///menus/GlobalShortcutsMedialib.qml"
    }
}
