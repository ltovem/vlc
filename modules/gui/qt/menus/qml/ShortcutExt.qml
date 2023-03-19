// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1

Shortcut {
    context: Qt.ApplicationShortcut
    enabled: MainCtx.useGlobalShortcuts
}
