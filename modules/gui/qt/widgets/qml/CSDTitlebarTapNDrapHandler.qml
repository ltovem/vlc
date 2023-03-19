// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/

//CSD is only supported on Qt 5.15
import QtQuick 2.15
import QtQuick.Window 2.15

import org.videolan.vlc 0.1

Item {
    TapHandler {
        onDoubleTapped: {

                if ((MainCtx.intfMainWindow.visibility & Window.Maximized) !== 0) {
                    MainCtx.requestInterfaceNormal()
                } else {
                    MainCtx.requestInterfaceMaximized()
                }

        }
        gesturePolicy: TapHandler.DragThreshold
    }
    DragHandler {
        target: null
        grabPermissions: TapHandler.CanTakeOverFromAnything
        onActiveChanged: {
            if (active) {
                MainCtx.intfMainWindow.startSystemMove();
            }
        }
    }

}
