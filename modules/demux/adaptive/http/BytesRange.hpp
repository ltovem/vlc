// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * BytesRange.hpp
 *****************************************************************************
 Copyright (C) 2015 - VideoLAN Authors
 *****************************************************************************/
#ifndef BYTESRANGE_HPP
#define BYTESRANGE_HPP

#include <vlc_common.h>

namespace adaptive
{
    namespace http
    {
        class BytesRange
        {
            public:
                BytesRange();
                BytesRange(size_t start, size_t end);
                bool isValid() const;
                size_t getStartByte() const;
                size_t getEndByte() const;

            private:
                size_t bytesStart;
                size_t bytesEnd;
        };
    }
}

#endif // BYTESRANGE_HPP
