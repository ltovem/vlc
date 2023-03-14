/*
 * ID.cpp
 *****************************************************************************
 * Copyright (C) 2015 - VideoLAN and VLC authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "ID.hpp"
#include <sstream>

using namespace adaptive;

ID::ID(const std::string &id_)
{
    id = id_;
}

ID::ID(uint64_t id_)
{
    std::stringstream ss;
    ss.imbue(std::locale("C"));
    ss << "default_id#" << id_;
    id = ss.str();
}

bool ID::isValid() const
{
    return !id.empty();
}

bool ID::operator==(const ID &other) const
{
    return (!id.empty() && id == other.id);
}

bool ID::operator<(const ID &other) const
{
    return (id.compare(other.id) < 0);
}

std::string ID::str() const
{
    return id;
}

const ID & Unique::getID() const
{
    return id;
}

void Unique::setID(const ID &id_)
{
    id = id_;
}
