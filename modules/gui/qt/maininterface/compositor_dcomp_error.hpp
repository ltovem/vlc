/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef COMPOSITOR_DCOMP_ERROR_HPP
#define COMPOSITOR_DCOMP_ERROR_HPP


#include <stdexcept>
#include <windows.h>

namespace vlc {

class DXError : public std::runtime_error
{
public:
    explicit DXError(const std::string& msg, HRESULT code)
        : std::runtime_error(msg)
        , m_code(code)
    {
    }

    explicit DXError(const char* msg, HRESULT code)
        : std::runtime_error(msg)
        , m_code(code)
    {
    }

    inline HRESULT code() const
    {
        return m_code;
    }

private:
    HRESULT m_code;
};

inline void HR( HRESULT hr, const std::string& msg )
{
    if( FAILED( hr ) )
        throw DXError{ msg, hr };
}

inline void HR( HRESULT hr, const char* msg = "" )
{
    if( FAILED( hr ) )
        throw DXError{ msg, hr  };
}

}


#endif // COMPOSITOR_DCOMP_ERROR_HPP
