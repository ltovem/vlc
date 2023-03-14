/*
 * Profile.cpp
 *****************************************************************************
 * Copyright (C) 2010 - 2014 VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "Profile.hpp"

using namespace dash::mpd;

struct
{
    const Profile::Name name;
    const char * urn;
}
const urnmap[] =
{
    { Profile::Name::Full,         "urn:mpeg:dash:profile:full:2011" },
    { Profile::Name::ISOOnDemand,  "urn:mpeg:dash:profile:isoff-on-demand:2011" },
    { Profile::Name::ISOOnDemand,  "urn:mpeg:mpegB:profile:dash:isoff-basic-on-demand:cm" },
    { Profile::Name::ISOOnDemand,  "urn:mpeg:dash:profile:isoff-ondemand:2011" },
    { Profile::Name::ISOMain,      "urn:mpeg:dash:profile:isoff-main:2011" },
    { Profile::Name::ISOLive,      "urn:mpeg:dash:profile:isoff-live:2011" },
    { Profile::Name::MPEG2TSMain,  "urn:mpeg:dash:profile:mp2t-main:2011" },
    { Profile::Name::MPEG2TSSimple,"urn:mpeg:dash:profile:mp2t-simple:2011" },
    { Profile::Name::Unknown,      "" },
};

Profile::Profile(Name name)
{
    type = name;
}

Profile::Profile(const std::string &urn)
{
    type = getNameByURN(urn);
}

Profile::Name Profile::getNameByURN(const std::string &urn) const
{
    for( int i=0; urnmap[i].name != Name::Unknown; i++ )
    {
        if ( urn == urnmap[i].urn )
            return urnmap[i].name;
    }
    return Name::Unknown;
}

Profile::operator Profile::Name ()
{
    return type;
}

Profile::operator std::string ()
{
    for( int i=0; urnmap[i].name != Name::Unknown; i++ )
    {
        if ( urnmap[i].name == type )
            return std::string( urnmap[i].urn );
    }
    return std::string();
}

bool Profile::operator==(Profile &profile) const
{
    return profile.type == type;
}
