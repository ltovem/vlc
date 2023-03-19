// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * DASHManager.h
 *****************************************************************************
 Copyright © 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef DASHMANAGER_H_
#define DASHMANAGER_H_

#include "../adaptive/PlaylistManager.h"
#include "../adaptive/logic/AbstractAdaptationLogic.h"
#include "mpd/MPD.h"

namespace adaptive
{
    namespace xml
    {
        class Node;
    }
}

namespace dash
{
    using namespace adaptive;

    class DASHManager : public PlaylistManager
    {
        public:
            DASHManager( demux_t *,
                         SharedResources *,
                         mpd::MPD *mpd,
                         AbstractStreamFactory *,
                         logic::AbstractAdaptationLogic::LogicType type);
            virtual ~DASHManager    ();

            virtual bool needsUpdate() const override;
            virtual bool updatePlaylist() override;
            virtual void scheduleNextUpdate() override;
            static bool isDASH(xml::Node *);
            static bool mimeMatched(const std::string &);

        protected:
            virtual int doControl(int, va_list) override;
    };

}

#endif /* DASHMANAGER_H_ */
