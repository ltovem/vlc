/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
import QtQuick 2.11

import org.videolan.vlc 0.1
import org.videolan.medialib 0.1

Item {
    ShortcutExt{ sequence:"Ctrl+B"; onActivated: DialogsProvider.bookmarksDialog() }

    MLRecentModel {
        id: recentModel
        numberOfItemsToShow: 10
        ml: MediaLib
    }

    //build all the shortcuts everytime, it seems that they can't be added/removed dynamically
    Repeater {
        model: 10

        Item {
            ShortcutExt {
                sequence: "Ctrl+" + ((index + 1) % 10)
                onActivated:  {
                    if (index < recentModel.count)
                    {

                        var trackId = recentModel.data(recentModel.index(index, 0), MLRecentModel.RECENT_MEDIA_ID)
                        if (!!trackId)
                            MediaLib.addAndPlay([trackId])
                    }
                }
            }
        }
    }
}
