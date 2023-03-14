/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import "qrc:///style/"

BusyIndicator {
    id: control

    property color color
    palette.text: color
    running: false

    property int delay: VLCStyle.duration_humanMoment
    property bool runningDelayed: false
    onRunningDelayedChanged: {
        if (runningDelayed) {
            controlDelay.start()
        } else {
            controlDelay.stop()
            control.running = false
        }
    }

    Timer {
        id: controlDelay
        interval: control.delay
        running: false
        repeat: false
        onTriggered: control.running = true
    }
}
