// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Profile.hpp
 *****************************************************************************
 Copyright (C) 2010 - 2014 VideoLAN and VLC Authors
 *****************************************************************************/
#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <string>

namespace dash
{
    namespace mpd
    {
        class Profile
        {
            public:
                enum class Name
                {
                    Unknown,
                    Full,
                    ISOOnDemand,
                    ISOMain,
                    ISOLive,
                    MPEG2TSMain,
                    MPEG2TSSimple,
                };
                Profile(Name);
                Profile(const std::string &);
                bool operator==(Profile &) const;
                operator Profile::Name ();
                operator std::string ();

            private:
                Name getNameByURN(const std::string &) const;
                Name type;
        };
    }
}
#endif // PROFILE_HPP
