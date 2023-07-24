/*****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
import QtQuick 2.12

import org.videolan.vlc 0.1

import "qrc:///util/Helpers.js" as Helpers

MouseArea {
    onWheel: {
        let delta = 0, fineControl = false

        if ((Math.abs(wheel.pixelDelta.x) % 120 > 0) || (Math.abs(wheel.pixelDelta.y) % 120 > 0)) {
            if (Math.abs(wheel.pixelDelta.x) > Math.abs(wheel.pixelDelta.y))
                delta = wheel.pixelDelta.x
            else
                delta = wheel.pixelDelta.y
            fineControl = true
        }
        else if (wheel.angleDelta.x)
            delta = wheel.angleDelta.x
        else if (wheel.angleDelta.y)
            delta = wheel.angleDelta.y

        if (delta === 0)
            return

        if (wheel.inverted)
            delta = -delta

        if (fineControl)
            root.value += 0.001 * delta
        else
            Helpers.applyVolume(Player, delta)

        wheel.accepted = true
    }
}
