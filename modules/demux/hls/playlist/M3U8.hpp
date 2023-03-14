/*
 * M3U8.hpp
 *****************************************************************************
 * Copyright © 2015 - VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef M3U8_H_
#define M3U8_H_

#include "../../adaptive/playlist/BasePlaylist.hpp"

namespace hls
{
    namespace playlist
    {
        using namespace adaptive::playlist;

        class M3U8 : public BasePlaylist
        {
            public:
                M3U8(vlc_object_t *);
                virtual ~M3U8();

                virtual bool isLive() const override;
        };
    }
}
#endif /* M3U8_H_ */
