// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Layouts 1.11
import org.videolan.vlc 0.1

import "qrc:///style/"
import "qrc:///widgets/" as Widgets

ModalDialog {
    id: root

    property string text

    property alias cancelTxt: cancelBtn.text
    property alias okTxt: okBtn.text

    property var _acceptCb: undefined
    property var _rejectCb: undefined

    function ask(text, acceptCb, rejectCb, buttons) {
        //TODO: use a Promise here when dropping support of Qt 5.11
        var okTxt = I18n.qtr("OK")
        var cancelTxt = I18n.qtr("cancel")
        if (buttons) {
            if (buttons.cancel) {
                cancelTxt = buttons.cancel
            }
            if (buttons.ok) {
                okTxt = buttons.ok
            }
        }
        root.cancelTxt = cancelTxt
        root.okTxt = okTxt
        root.text = text
        root._acceptCb = acceptCb
        root._rejectCb = rejectCb
        root.open()
    }

    onAccepted: {
        if (_acceptCb)
            _acceptCb()
    }

    onRejected: {
        if (_rejectCb)
            _rejectCb()
    }

    contentItem: Text {
        focus: false
        font.pixelSize: VLCStyle.fontSize_normal
        color: root.colorContext.fg.primary
        wrapMode: Text.WordWrap
        text: root.text
    }

    footer: FocusScope {
        focus: true
        id: questionButtons
        implicitHeight: VLCStyle.icon_normal

        readonly property ColorContext colorContext: ColorContext {
            id: footerTheme
            palette: root.colorContext.palette
            colorSet: ColorContext.Window
        }

        Rectangle {
            color: footerTheme.bg.primary
            anchors.fill: parent
            anchors.leftMargin: VLCStyle.margin_xxsmall
            anchors.rightMargin: VLCStyle.margin_xxsmall

            RowLayout {
                anchors.fill: parent

                Widgets.TextToolButton {
                    id: cancelBtn
                    Layout.fillWidth: true
                    focus: true
                    visible: cancelBtn.text !== ""

                    onClicked: root.reject()

                    Navigation.rightItem: okBtn
                    Keys.priority: Keys.AfterItem
                    Keys.onPressed: okBtn.Navigation.defaultKeyAction(event)
                }

                Widgets.TextToolButton {
                    id: okBtn
                    Layout.fillWidth: true
                    visible: okBtn.text !== ""

                    onClicked: root.accept()

                    Navigation.leftItem: cancelBtn
                    Keys.priority: Keys.AfterItem
                    Keys.onPressed: cancelBtn.Navigation.defaultKeyAction(event)
                }
            }
        }
    }
}
