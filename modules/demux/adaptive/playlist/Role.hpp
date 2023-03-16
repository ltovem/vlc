// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * Role.hpp
 *****************************************************************************
 * Copyright (C) 2019 VideoLabs, VideoLAN and VLC Authors
 *****************************************************************************/
#ifndef ROLE_HPP_
#define ROLE_HPP_

namespace adaptive
{
    namespace playlist
    {
        class Role
        {
            public:
                enum class Value
                {
                    Main,
                    Alternate,
                    Supplementary,
                    Commentary,
                    Dub,
                    Caption,
                    Subtitle,
                };
                Role(Value = Value::Main);
                bool operator<(const Role &) const;
                bool operator==(const Role &) const;
                bool isDefault() const;
                bool autoSelectable() const;

            private:
                Value value;
        };
    }
}

#endif
