/*
 * Manifest.hpp
 *****************************************************************************
 * Copyright © 2015 - VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef MANIFEST_HPP
#define MANIFEST_HPP

#include "../../adaptive/playlist/BasePlaylist.hpp"

namespace smooth
{
    namespace playlist
    {
        using namespace adaptive::playlist;

        class Manifest : public BasePlaylist
        {
            friend class ManifestParser;

            public:
                Manifest(vlc_object_t *);
                virtual ~Manifest();

                virtual bool                    isLive() const override;

            private:
                bool b_live;
        };
    }
}

#endif // MANIFEST_HPP
