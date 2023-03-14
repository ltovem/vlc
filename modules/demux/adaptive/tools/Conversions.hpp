/*
 * Conversions.hpp
 *****************************************************************************
 * Copyright © 2015 - VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef CONVERSIONS_HPP
#define CONVERSIONS_HPP

#include <vlc_common.h>
#include <string>
#include <sstream>

class IsoTime
{
    public:
        IsoTime(const std::string&);
        operator vlc_tick_t() const;

    private:
        vlc_tick_t time;
};

class UTCTime
{
    public:
        UTCTime(const std::string&);
        vlc_tick_t mtime() const;

    private:
        vlc_tick_t t;
};

template<typename T> class Integer
{
    public:
        Integer(const std::string &str)
        {
            try
            {
                std::istringstream in(str);
                in.imbue(std::locale("C"));
                in >> value;
                if (in.fail() || in.bad())
                    value = 0;
            } catch (...) {
                value = 0;
            }
        }

        operator T() const
        {
            return value;
        }

    private:
        T value;
};

#endif // CONVERSIONS_HPP
