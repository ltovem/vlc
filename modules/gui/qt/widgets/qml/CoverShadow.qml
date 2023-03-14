
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtGraphicalEffects 1.0

import "qrc:///style/"

Item {
    id: root

    property alias cached: primaryShadow.cached
    property alias source: primaryShadow.source

    property alias primaryColor: primaryShadow.color
    property alias primaryVerticalOffset: primaryShadow.verticalOffset
    property alias primaryHorizontalOffset: primaryShadow.horizontalOffset
    property alias primaryBlurRadius: primaryShadow.radius
    property alias primarySamples: primaryShadow.samples

    property alias secondaryColor: secondaryShadow.color
    property alias secondaryVerticalOffset: secondaryShadow.verticalOffset
    property alias secondaryHorizontalOffset: secondaryShadow.horizontalOffset
    property alias secondaryBlurRadius: secondaryShadow.radius
    property alias secondarySamples: secondaryShadow.samples

    DropShadow {
        id: primaryShadow

        anchors.fill: parent
        horizontalOffset: 0
        spread: 0
        color: Qt.rgba(0, 0, 0, .22)
        samples: 1 + radius * 2
    }

    DropShadow {
        id: secondaryShadow

        anchors.fill: parent
        source: primaryShadow.source
        horizontalOffset: 0
        spread: 0
        color: Qt.rgba(0, 0, 0, .18)
        samples: 1 + radius * 2
        cached: root.cached
    }
}
