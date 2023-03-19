// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *****************************************************************************/

import QtQuick 2.11

import "qrc:///style/"

Loader {
    id: root

    // Settings

    // NOTE: We could consider applying 'layer.enabled' during transitions since it removes overlap
    //       artifacts when applying opacity.

    // States

    states: [
        State {
            name: "hidden"

            PropertyChanges {
                target: root

                visible: false
                opacity: 0.0
            }
        },
        State {
            name: "visible"

            PropertyChanges {
                target: root

                visible: true
                opacity: 1.0
            }
        }
    ]

    transitions: [
        Transition {
            to: "hidden"

            SequentialAnimation {
                NumberAnimation {
                    target: root

                    property: "opacity"

                    duration: VLCStyle.duration_long
                    easing.type: Easing.InSine
                }

                PropertyAction{
                    target: root

                    property: "visible"
                }
            }
        },
        Transition {
            to: "visible"

            SequentialAnimation {
                PropertyAction {
                    target: root

                    property: "visible"
                }

                NumberAnimation {
                    target: root

                    property: "opacity"

                    duration: VLCStyle.duration_long
                    easing.type: Easing.OutSine
                }
            }
        }
    ]
}
