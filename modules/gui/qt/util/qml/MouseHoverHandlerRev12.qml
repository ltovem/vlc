/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.12

// Due to a bug ( https://bugreports.qt.io/browse/QTBUG-85427 )
// this handler might react to touch events

HoverHandler {
    acceptedDevices: PointerDevice.Mouse
}
