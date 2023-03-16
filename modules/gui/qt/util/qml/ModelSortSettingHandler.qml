
// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *****************************************************************************/
import QtQml 2.11

import org.videolan.vlc 0.1

QtObject {
    id: root

    property var _model: null
    property string _key: ""

    readonly property string _sortCriteriaKey: "sortCriteria/" + _key
    readonly property string _sortOrderKey: "sortOrder/" + _key

    property var _sortCriteriaConnection: Connections {
        target: !!root._model ? root._model : null

        enabled: !!root._model && root._model.hasOwnProperty("sortCriteria")

        onSortCriteriaChanged: {
            MainCtx.setSettingValue(root._sortCriteriaKey, _model.sortCriteria)
        }
    }

    property var _sortOrderConnection: Connections {
        target: !!root._model ? root._model : null

        enabled: !!root._model && root._model.hasOwnProperty("sortOrder")

        onSortOrderChanged: {
            MainCtx.setSettingValue(root._sortOrderKey, root._model.sortOrder)
        }
    }

    function set(model, key) {
        _model = model
        _key = key

        if (!_model)
            return

        if (_model.hasOwnProperty("sortCriteria"))
            _model.sortCriteria = MainCtx.settingValue(_sortCriteriaKey, _model.sortCriteria)

        // MainCtx.settingValue seems to change int -> string
        if (_model.hasOwnProperty("sortOrder"))
            _model.sortOrder = parseInt(MainCtx.settingValue(_sortOrderKey, _model.sortOrder))
    }
}
