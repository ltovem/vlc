
// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************
 * Author: Prince Gupta <guptaprince8832@gmail.com>
 *****************************************************************************/

import QtQuick 2.11

import org.videolan.medialib 0.1

DragItem {
    id: root

    /* required */ property MLModel mlModel: null

    // string => role for medialib id, data[id] will be pass to Medialib::mlInputItem for QmlInputItem
    property string mlIDRole: "id"

    function getSelectedInputItem(cb) {
        console.assert(mlIDRole)
        var inputIdList = root.indexesData.map(function(obj){
            return obj[root.mlIDRole]
        })
        MediaLib.mlInputItem(inputIdList, cb)
    }

    onRequestData: {
        mlModel.getData(indexes, function (data) {
            root.setData(identifier, data)
        })
    }
}
