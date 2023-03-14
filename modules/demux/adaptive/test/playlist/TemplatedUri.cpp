/*****************************************************************************
 * dashuri.cpp
 *****************************************************************************
 * Copyright (C) 2018 VideoLabs, VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "../test.hpp"

#include "../../../dash/mpd/TemplatedUri.hpp"

#include <cstring>
#include <vlc_common.h>

using namespace dash::mpd;

static const struct
{
    const char *src;
    const char *dst;
    const char *str;
    const unsigned val;
} dataset[] = {
    {
        "",
        "",
        nullptr,
        0,
    },
    {
        "$",
        "$",
        nullptr,
        0,
    },
    {
        "/Num$$ber.m4v",
        "/Num$ber.m4v",
        nullptr,
        0,
    },
    {
        "/$Number$.m4v",
        "/123.m4v",
        nullptr,
        123,
    },
    {
        "/$$$Number$.m4v",
        "/$456789123.m4v",
        nullptr,
        456789123,
    },
    {
        "$Number%d$",
        "123",
        nullptr,
        123,
    },
    {
        "/$Number%5d$.m4v",
        "/00001.m4v",
        nullptr,
        1,
    },
    {
        "/$Number%2d$.m4v",
        "/123456.m4v",
        nullptr,
        123456, /* Must not truncate */
    },
    {
        "/$RepresentationID$.m4v",
        "/foobar.m4v",
        "foobar",
        0,
    },
    {
        "/$RepresentationID$.m4v",
        "/$Time$.m4v",
        "$Time$",
        0,
    },
    {
        "$RepresentationID$/$Number$$Time$$$",
        "id/123123$",
        "id",
        123, /* Must not truncate */
    },
};

int TemplatedUri_test()
{
    try {
        for(size_t i=0; i<ARRAY_SIZE(dataset); i++)
        {
            std::string str = std::string(dataset[i].src);

            std::cerr << str << std::endl;

            std::string::size_type pos = 0;
            while(pos < str.length())
            {
                TemplatedUri::Token token;

                if(str[pos] == '$' && TemplatedUri::IsDASHToken(str, pos, token))
                {
                    std::cerr << " * token " << str.substr(pos, token.fulllength)
                              << " " << token.width << std::endl;

                    TemplatedUri::TokenReplacement replparam;

                    switch(token.type)
                    {
                        case TemplatedUri::Token::TOKEN_TIME:
                        case TemplatedUri::Token::TOKEN_BANDWIDTH:
                        case TemplatedUri::Token::TOKEN_NUMBER:
                            replparam.value = dataset[i].val;
                            break;
                        case TemplatedUri::Token::TOKEN_REPRESENTATION:
                        {
                            Expect(dataset[i].str);
                            replparam.str = std::string(dataset[i].str);
                            break;
                        }
                        case TemplatedUri::Token::TOKEN_ESCAPE:
                            break;

                        default:
                            pos += token.fulllength;
                            continue;
                    }

                    std::string::size_type newlen =
                            TemplatedUri::ReplaceDASHToken(str, pos, token, replparam);
                    Expect(newlen != std::string::npos);
                    pos += newlen;
                }
                else pos++;
            }

            std::cerr << " -> " << str << std::endl;

            Expect(!std::strcmp(dataset[i].dst, str.c_str()));
        }

        return 0;
    }
    catch (...)
    {
        return 1;
    }
}
