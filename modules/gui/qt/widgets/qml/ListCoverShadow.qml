
// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11
import QtGraphicalEffects 1.0

import "qrc:///style/"

DoubleShadow {
    id: root

    primaryVerticalOffset: VLCStyle.dp(6, VLCStyle.scale)
    primaryBlurRadius: VLCStyle.dp(14, VLCStyle.scale)

    secondaryVerticalOffset: VLCStyle.dp(1, VLCStyle.scale)
    secondaryBlurRadius: VLCStyle.dp(3, VLCStyle.scale)

    z: -1
}
