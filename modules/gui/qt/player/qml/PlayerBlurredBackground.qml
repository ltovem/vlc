// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *****************************************************************************/

import QtQuick 2.11

import QtGraphicalEffects 1.0

Item {
    id: rootItem

    property alias source: blurSource.sourceItem
    property alias screenColor: shaderItem.screenColor
    property alias overlayColor: shaderItem.overlayColor

    function scheduleUpdate() {
        blurSource.scheduleUpdate()
    }

    onSourceChanged: blurSource.scheduleUpdate()

    ShaderEffectSource {
        id: blurSource

        width: 512
        height: 512
        textureSize: Qt.size(512,512)

        visible: false

        live: false
        hideSource: false
        smooth: false
        mipmap: false
    }

    FastBlur {
        id: blur

        source: blurSource

        width: 512
        height: 512

        radius: 50
        visible: false
    }

    ShaderEffectSource {
        id: proxySource
        live: true
        hideSource: false
        sourceItem: blur
        smooth: false
        visible: false
        mipmap: false
    }

    ShaderEffect {
        id: shaderItem

        property var backgroundSource: proxySource
        property color screenColor
        property color overlayColor

        anchors.fill: parent
        visible: true
        blending: false
        cullMode: ShaderEffect.BackFaceCulling

        fragmentShader: "qrc:///player/PlayerBlurredBackground.frag"
    }
}
