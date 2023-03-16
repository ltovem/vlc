
// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *****************************************************************************/
import QtQuick 2.11

QtObject {
    id: object

    default property alias children: object.__children

    property list<QtObject> __children: [QtObject {}]
}
