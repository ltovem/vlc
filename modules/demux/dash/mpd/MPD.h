// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * MPD.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef MPD_H_
#define MPD_H_

#include "../../adaptive/playlist/BasePlaylist.hpp"
#include "../../adaptive/StreamFormat.hpp"
#include "Profile.hpp"

namespace dash
{
    namespace mpd
    {
        using namespace adaptive::playlist;
        using namespace adaptive;

        class ProgramInformation;

        class MPD : public BasePlaylist
        {
            friend class IsoffMainParser;

            public:
                MPD(vlc_object_t *, Profile);
                virtual ~MPD();

                Profile                         getProfile() const;
                virtual bool                    isLive() const override;
                virtual bool                    isLowLatency() const override;
                void                            setLowLatency(bool);
                virtual void                    debug() const override;

                Property<ProgramInformation *>      programInfo;

            private:
                Profile                             profile;
                bool                                lowLatency;
        };
    }
}
#endif /* MPD_H_ */
