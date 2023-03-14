/*
 * Properties.hpp
 *****************************************************************************
 * Copyright (C) 2014 - VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

template <typename T> class Property
{
    public:
        Property() {}

        T & Set(const T &i)
        {
            return value = i;
        }

        T const & Get() const
        {
            return value;
        }

    private:
        T value;
};

template <typename T> class Undef
{
    public:
        Undef() { undef = true; }

        bool isSet() const
        {
            return !undef;
        }

        void operator =(const T &v) { val = v; undef = false; }
        const T& value() const { return val; }

    private:
        bool undef;
        T val;
};

template <typename T> class Ratio
{
    public:
        Ratio() { m_den = m_num = 0; }
        Ratio(T n, T d) { m_den = d, m_num = n; }
        bool isValid() const { return m_num && m_den; }
        T num() const { return m_num; }
        T den() const { return m_den; }
        T width() const { return m_num; }
        T height() const { return m_den; }

    protected:
        T m_num;
        T m_den;
};

using AspectRatio = Ratio<unsigned int>;
using Rate = Ratio<unsigned int>;

#endif // PROPERTIES_HPP
