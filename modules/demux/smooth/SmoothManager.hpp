/*
 * SmoothManager.hpp
 *****************************************************************************
 * Copyright © 2015 - VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef SMOOTHMANAGER_HPP
#define SMOOTHMANAGER_HPP

#include "../adaptive/PlaylistManager.h"
#include "../adaptive/logic/AbstractAdaptationLogic.h"
#include "playlist/Manifest.hpp"

namespace adaptive
{
    namespace xml
    {
        class Node;
    }
}

namespace smooth
{
    using namespace adaptive;

    class SmoothManager : public PlaylistManager
    {
        public:
            SmoothManager( demux_t *, SharedResources *, playlist::Manifest *,
                        AbstractStreamFactory *,
                        logic::AbstractAdaptationLogic::LogicType type );
            virtual ~SmoothManager();

            virtual bool needsUpdate() const override;
            virtual void scheduleNextUpdate() override;
            virtual bool updatePlaylist() override;

            static bool isSmoothStreaming(xml::Node *);
            static bool mimeMatched(const std::string &);

        protected:
            virtual bool reactivateStream(AbstractStream *) override;

        private:
            bool updatePlaylist(bool);
            playlist::Manifest * fetchManifest();
    };

}

#endif // SMOOTHMANAGER_HPP
