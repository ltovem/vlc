/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Templates 2.4 as T
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11

import org.videolan.vlc 0.1

import "qrc:///style/"

T.AbstractButton {
    id: control

    padding: 0
    implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding
    implicitHeight: contentItem.implicitHeight + topPadding + bottomPadding
    focusPolicy: Qt.NoFocus

    property int buttonType: CSDThemeImage.CLOSE
    property int bannerHeight: -1
    property bool showBg: false

    background: null

    contentItem: CSDThemeImage {

        anchors.verticalCenter: parent.verticalCenter

        bannerHeight: control.bannerHeight

        theme: VLCStyle.palette

        windowMaximized: MainCtx.intfMainWindow.visibility === Window.Maximized
        windowActive: MainCtx.intfMainWindow.active

        buttonType: control.buttonType
        buttonState: {
            if (control.pressed)
                return CSDThemeImage.PRESSED
            else if (control.hovered)
                return CSDThemeImage.HOVERED
            else
                return CSDThemeImage.NORMAL
        }
    }
}
