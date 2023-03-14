/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11

import "qrc:///style/"

// A convenience file to encapsulate two drop shadow images stacked on top
// of each other
Item {
    id: root

    property var xRadius: null
    property var yRadius: null

    property alias primaryVerticalOffset: primaryShadow.yOffset
    property alias primaryHorizontalOffset: primaryShadow.xOffset
    property alias primaryColor: primaryShadow.color
    property alias primaryBlurRadius: primaryShadow.blurRadius
    property alias primaryXRadius: primaryShadow.xRadius
    property alias primaryYRadius: primaryShadow.yRadius

    property alias secondaryVerticalOffset: secondaryShadow.yOffset
    property alias secondaryHorizontalOffset: secondaryShadow.xOffset
    property alias secondaryColor: secondaryShadow.color
    property alias secondaryBlurRadius: secondaryShadow.blurRadius
    property alias secondaryXRadius: secondaryShadow.xRadius
    property alias secondaryYRadius: secondaryShadow.yRadius

    property alias cache: primaryShadow.cache

    visible: (width > 0 && height > 0)

    DropShadowImage {
        id: primaryShadow

        anchors.centerIn: parent

        color: Qt.rgba(0, 0, 0, .18)
        xOffset: 0

        xRadius: root.xRadius
        yRadius: root.yRadius

        sourceSize: Qt.size(parent.width, parent.height)
    }

    DropShadowImage {
        id: secondaryShadow

        anchors.centerIn: parent

        color: Qt.rgba(0, 0, 0, .22)
        xOffset: 0

        xRadius: root.xRadius
        yRadius: root.yRadius

        sourceSize: Qt.size(parent.width, parent.height)

        cache: root.cache
    }
}
