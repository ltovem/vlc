// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * MPD.cpp
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cinttypes>

#include "MPD.h"
#include "ProgramInformation.h"

#include <vlc_common.h>
#include <vlc_stream.h>

using namespace dash::mpd;

MPD::MPD (vlc_object_t *p_object, Profile profile_) :
    BasePlaylist(p_object),
    profile( profile_ )
{
    programInfo.Set( nullptr );
    lowLatency = false;
}

MPD::~MPD()
{
    delete(programInfo.Get());
}

bool MPD::isLive() const
{
    if(type.empty())
    {
        Profile live(Profile::Name::ISOLive);
        return profile == live;
    }
    else
        return (type != "static");
}

bool MPD::isLowLatency() const
{
    return lowLatency;
}

void MPD::setLowLatency(bool b)
{
    lowLatency = b;
}

Profile MPD::getProfile() const
{
    return profile;
}

void MPD::debug() const
{
    msg_Dbg(p_object, "MPD profile=%s mediaPresentationDuration=%" PRId64
            " minBufferTime=%" PRId64,
            static_cast<std::string>(getProfile()).c_str(),
            SEC_FROM_VLC_TICK(duration.Get()),
            minBufferTime);
    msg_Dbg(p_object, "BaseUrl=%s", getUrlSegment().toString().c_str());

    BasePlaylist::debug();
}
