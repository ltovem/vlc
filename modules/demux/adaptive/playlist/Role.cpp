// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * Role.cpp
 *****************************************************************************
 * Copyright (C) 2019 VideoLabs, VideoLAN and VLC Authors
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "Role.hpp"

using namespace adaptive;
using namespace adaptive::playlist;

Role::Role(Value r)
{
    value = r;
}

bool Role::operator <(const Role &other) const
{
    return value > other.value;
}

bool Role::operator ==(const Role &other) const
{
    return value == other.value;
}

bool Role::isDefault() const
{
    return value == Value::Main;
}

bool Role::autoSelectable() const
{
    return value == Value::Main ||
           value == Value::Alternate ||
           value == Value::Subtitle ||
           value == Value::Caption;
}
