/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.15
import QtQuick.Window 2.15

import org.videolan.vlc 0.1

Item {
    id: root

    property int csdSize: MainCtx.csdBorderSize

    //private
    readonly property int _edgeVtHeight: g_root.height - root.csdSize * 2
    readonly property int _edgeHzWidth: g_root.width - root.csdSize * 2

    Repeater {
        model: [
            //Edges
            {
                edge: Qt.TopEdge,
                x: root.csdSize,
                y: 0,
                width: root._edgeHzWidth,
                height: root.csdSize,
                cursor: Qt.SizeVerCursor,
            },
            {
                edge: Qt.LeftEdge,
                x: 0,
                y: root.csdSize,
                width: root.csdSize,
                height: root._edgeVtHeight,
                cursor: Qt.SizeHorCursor,
            },
            {
                edge: Qt.RightEdge,
                x: g_root.width - root.csdSize,
                y: root.csdSize,
                width: root.csdSize,
                height: root._edgeVtHeight,
                cursor: Qt.SizeHorCursor,
            },
            {
                edge: Qt.BottomEdge,
                x: root.csdSize,
                y: g_root.height - root.csdSize,
                width: root._edgeHzWidth,
                height: root.csdSize,
                cursor: Qt.SizeVerCursor,
            },
            //Corners
            {
                edge: Qt.TopEdge | Qt.LeftEdge,
                x: 0,
                y: 0,
                width: root.csdSize,
                height: root.csdSize,
                cursor: Qt.SizeFDiagCursor,
            },
            {
                edge: Qt.BottomEdge | Qt.LeftEdge,
                x: 0,
                y: g_root.height - root.csdSize,
                width: root.csdSize,
                height: root.csdSize,
                cursor: Qt.SizeBDiagCursor,
            },
            {
                edge: Qt.TopEdge | Qt.RightEdge,
                x: g_root.width - root.csdSize,
                y: 0,
                width: root.csdSize,
                height: root.csdSize,
                cursor: Qt.SizeBDiagCursor,
            },
            {
                edge: Qt.BottomEdge | Qt.RightEdge,
                x: g_root.width - root.csdSize,
                y: g_root.height - root.csdSize,
                width: root.csdSize,
                height: root.csdSize,
                cursor: Qt.SizeFDiagCursor,
            },
        ]

        delegate: MouseArea {
            x: modelData.x
            y: modelData.y

            width: modelData.width
            height: modelData.height

            hoverEnabled: true
            cursorShape: modelData.cursor
            acceptedButtons: Qt.LeftButton

            onPressed: MainCtx.intfMainWindow.startSystemResize(modelData.edge)
        }
    }
}
