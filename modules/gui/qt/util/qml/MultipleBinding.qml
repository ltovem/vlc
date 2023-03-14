/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

import QtQuick 2.11
import QtQml 2.11
import QtQml.Models 2.11

import org.videolan.compat 0.1

QtObject {
    id: root

    property ListModel model
    property alias enabled: instantiator.active
    property alias asynchronous: instantiator.asynchronous

    property QtObject target: null
    property bool when
    property bool delayed: false

    readonly property QtObject _instantiator: Instantiator {
        id: instantiator

        model: root.model

        delegate: BindingCompat {
            target: model.target ? model.target
                                 : root.target
            when: model.when !== undefined ? model.when
                                           : root.when
            property: model.property
            value: model.value
            delayed: model.delayed !== undefined ? model.delayed
                                                 : root.delayed
        }
    }
}
