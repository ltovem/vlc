/*
 * TemplatedUri.hpp
 *****************************************************************************
 * Copyright (C) 2010 - 2014 VideoLAN and VLC Authors
 *               2018        VideoLabs
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef TEMPLATEDURI_HPP
#define TEMPLATEDURI_HPP

#include <cstdint>
#include <string>

namespace dash
{
    namespace mpd
    {
        class TemplatedUri
        {
            public:
                class Token
                {
                    public:
                        enum tokentype
                        {
                            TOKEN_ESCAPE,
                            TOKEN_TIME,
                            TOKEN_BANDWIDTH,
                            TOKEN_REPRESENTATION,
                            TOKEN_NUMBER,
                        } type;

                        std::string::size_type fulllength;
                        int width;
                };

                static bool IsDASHToken(const std::string &str,
                                        std::string::size_type pos,
                                        TemplatedUri::Token &token);

                class TokenReplacement
                {
                    public:
                        uint64_t value;
                        std::string str;
                };

                static std::string::size_type
                            ReplaceDASHToken(std::string &str,
                                             std::string::size_type pos,
                                             const TemplatedUri::Token &token,
                                             const TemplatedUri::TokenReplacement &repl);
        };
    }
}
#endif // TEMPLATEDURI_HPP
