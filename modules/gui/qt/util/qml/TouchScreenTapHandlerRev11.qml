/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11

Item {
    //mock, TapHandler is present in QtQuick >= 2.12
    signal tapped()
    signal longPressed()
}
