/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11

import org.videolan.vlc 0.1

import "qrc:///style/"

Rectangle {
    id: root

    readonly property bool hovered: minimizeButton.hovered || maximizeButton.hovered || closeButton.hovered

    readonly property int _frameMarginLeft: VLCStyle.palette.csdMetrics ? VLCStyle.palette.csdMetrics.csdFrameMarginLeft : 0
    readonly property int _frameMarginRight: VLCStyle.palette.csdMetrics ? VLCStyle.palette.csdMetrics.csdFrameMarginRight : 0
    readonly property int _interNavButtonSpacing: VLCStyle.palette.csdMetrics ? VLCStyle.palette.csdMetrics.interNavButtonSpacing : 0

    implicitWidth: layout.implicitWidth + _frameMarginLeft + _frameMarginRight
    implicitHeight: layout.implicitHeight

    color: "transparent"

    Row {
        id: layout

        anchors.fill: parent
        anchors.leftMargin: root._frameMarginLeft
        anchors.rightMargin: root._frameMarginRight

        spacing: root._interNavButtonSpacing

        Repeater {
            model: MainCtx.csdButtonModel.windowCSDButtons

            CSDThemeButton {

                anchors.verticalCenter: parent.verticalCenter

                bannerHeight: root.height

                buttonType: {
                    switch (modelData.type) {
                    case CSDButton.Minimize:
                        return CSDThemeImage.MINIMIZE

                    case CSDButton.MaximizeRestore:
                        return (MainCtx.intfMainWindow.visibility === Window.Maximized)
                                ? CSDThemeImage.RESTORE
                                : CSDThemeImage.MAXIMIZE

                    case CSDButton.Close:
                        return CSDThemeButton.CLOSE
                    }

                    console.assert(false, "unreachable")
                }

                onClicked: modelData.click()
            }
        }
    }
}
