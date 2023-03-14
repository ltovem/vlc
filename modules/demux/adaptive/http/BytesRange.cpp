/*
 * BytesRange.cpp
 *****************************************************************************
 * Copyright (C) 2015 - VideoLAN Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "BytesRange.hpp"

using namespace adaptive::http;

BytesRange::BytesRange()
{
    bytesStart = 2;
    bytesEnd = 1;
}

BytesRange::BytesRange(size_t start, size_t end)
{
    bytesStart = start;
    bytesEnd = end;
}

bool BytesRange::isValid() const
{
    if(bytesEnd < bytesStart)
        return bytesEnd == 0;
    return true;
}

size_t BytesRange::getStartByte() const
{
    return bytesStart;
}

size_t BytesRange::getEndByte() const
{
    return bytesEnd;
}
